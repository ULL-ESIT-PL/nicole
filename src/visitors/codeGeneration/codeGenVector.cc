#include "../../../inc/visitors/codeGeneration/codeGeneration.h"

#include "../../../inc/parsingAnalysis/ast/vector/ast_index.h"
#include "../../../inc/parsingAnalysis/ast/vector/ast_vector.h"

#include <cstddef>
#include <memory>
#include <variant>

namespace nicole {

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_VECTOR *node) const noexcept {
  if (!node)
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_VECTOR");

  // obtener VectorType y su StructType { T* data; i64 size; }
  auto vecType =
      std::dynamic_pointer_cast<VectorType>(node->returnedFromTypeAnalysis());
  if (!vecType)
    return createError(ERROR_TYPE::TYPE, "AST_VECTOR no retorna VectorType");
  auto structOrErr = vecType->llvmVersion(context_);
  if (!structOrErr)
    return createError(structOrErr.error());
  llvm::StructType *vecStruct = llvm::cast<llvm::StructType>(*structOrErr);

  // tipo de elemento y número de elementos
  auto elemTyOrErr = vecType->elementType()->llvmVersion(context_);
  if (!elemTyOrErr)
    return createError(elemTyOrErr.error());
  llvm::Type *elemTy = *elemTyOrErr;
  llvm::Value *numElems = llvm::ConstantInt::get(
      llvm::Type::getInt64Ty(context_), node->values().size());

  // crear allocas en entry
  llvm::AllocaInst *vecAlloca, *dataAlloca;
  {
    llvm::IRBuilder<> entryBuilder(entry_);
    auto ip = entry_->getFirstInsertionPt();
    entryBuilder.SetInsertPoint(entry_, ip);
    vecAlloca = entryBuilder.CreateAlloca(vecStruct, nullptr, "vec_literal");
    dataAlloca = entryBuilder.CreateAlloca(elemTy, numElems, "vec_data");
  }

  resultChainedExpression_ = vecAlloca;

  // rellenar buffer
  for (size_t i = 0; i < node->values().size(); ++i) {
    auto valOrErr = node->values()[i]->accept(*this);
    if (!valOrErr)
      return createError(valOrErr.error());
    llvm::Value *elemValPtr = *valOrErr;

    // apuntar al slot i
    llvm::Value *slotPtr = builder_.CreateInBoundsGEP(
        elemTy, dataAlloca, {builder_.getInt64(i)}, "vec_elem_gep");

    // si es struct (otro vector), copiarlo entero; si no, store directo
    if (elemTy->isAggregateType()) {
      llvm::Value *loadedStruct =
          builder_.CreateLoad(elemTy, elemValPtr, "agg_load");
      builder_.CreateStore(loadedStruct, slotPtr);
    } else {
      builder_.CreateStore(elemValPtr, slotPtr);
    }
  }

  // escribir campos .data y .size en el struct
  llvm::Value *dataField =
      builder_.CreateStructGEP(vecStruct, vecAlloca, 0, "vec_data_ptr");
  builder_.CreateStore(dataAlloca, dataField);

  llvm::Value *sizeField =
      builder_.CreateStructGEP(vecStruct, vecAlloca, 1, "vec_size_ptr");
  builder_.CreateStore(numElems, sizeField);

  // actualizar tipo y devolver
  currentType = vecType;
  return vecAlloca;
}

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_INDEX *node) const noexcept {
  if (!node)
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_INDEX");

  // Evaluar y extender índice a i64
  auto idxOrErr = node->index()->accept(*this);
  if (!idxOrErr)
    return createError(idxOrErr.error());
  llvm::Value *idxVal = *idxOrErr;
  if (idxVal->getType()->isIntegerTy(32)) {
    idxVal = builder_.CreateSExt(idxVal, llvm::Type::getInt64Ty(context_),
                                 "idx_to_i64");
  }

  // Obtener base del índice
  llvm::Value *basePtr = resultChainedExpression_;
  if (!basePtr || !basePtr->getType()->isPointerTy())
    return createError(ERROR_TYPE::TYPE, "base of index is not a pointer");

  // Determinar comportamiento según tipo actual
  llvm::Value *resultPtr = nullptr;

  // --- Caso VECTOR ---
  if (auto vecType = std::dynamic_pointer_cast<VectorType>(currentType)) {
    // Obtener StructType { T* data; i64 size; }
    auto structOrErr = vecType->llvmVersion(context_);
    if (!structOrErr)
      return createError(structOrErr.error());
    llvm::StructType *vecStruct = llvm::cast<llvm::StructType>(*structOrErr);

    // GEP a campo .data: índices {0,0}
    llvm::Value *dataPtrField = builder_.CreateInBoundsGEP(
        vecStruct, basePtr, {builder_.getInt64(0), builder_.getInt32(0)},
        "vec_data_gep");
    // Load del T* que apunta al buffer
    llvm::Type *tTy = vecType->elementType()->llvmVersion(context_).value();
    llvm::Value *dataPtr = builder_.CreateLoad(
        llvm::PointerType::getUnqual(tTy), dataPtrField, "vec_data_load");

    // GEP al elemento dentro del buffer
    llvm::Type *elemTy = tTy;
    llvm::Value *elemPtr =
        builder_.CreateInBoundsGEP(elemTy, dataPtr, {idxVal}, "vec_elem_gep");

    // Si el elemento es un struct (otro vector), devolvemos el ptr;
    //      si no, hacemos load
    if (elemTy->isAggregateType()) {
      resultPtr = elemPtr;
    } else {
      resultPtr = builder_.CreateLoad(elemTy, elemPtr, "idx_load");
    }

    // Actualizar estado
    resultChainedExpression_ = resultPtr;
    currentType = vecType->elementType();
    return resultPtr;
  }

  // --- Caso STRING ---
  if (auto basic = std::dynamic_pointer_cast<BasicType>(currentType);
      basic && basic->baseKind() == BasicKind::Str) {
    // 4.1) El puntero base es i8*: acceder con un único índice
    llvm::Type *charTy = llvm::Type::getInt8Ty(context_);
    llvm::Value *charPtr =
        builder_.CreateInBoundsGEP(charTy, basePtr, {idxVal}, "str_elem_gep");
    // Load del carácter
    llvm::LoadInst *loadedChar =
        builder_.CreateLoad(charTy, charPtr, "idx_load");

    // Actualizar estado y tipo
    resultChainedExpression_ = loadedChar;
    currentType = std::make_shared<BasicType>(BasicKind::Char);
    return loadedChar;
  }

  // --- Caso NO SOPORTADO ---
  return createError(ERROR_TYPE::TYPE,
                     "indexed type is neither vector nor string");
}

} // namespace nicole
#include "../../../inc/visitors/codeGeneration/codeGeneration.h"

#include "../../../inc/parsingAnalysis/ast/assignments/ast_assignment.h"
#include "../../../inc/parsingAnalysis/ast/variables/ast_varCall.h"
#include "../../../inc/parsingAnalysis/ast/vector/ast_index.h"
#include "../../../inc/parsingAnalysis/ast/chained/ast_chained.h"

#include <memory>

namespace nicole {

// Helper: obtiene siempre una dirección (T*) válida para el LHS
std::expected<llvm::Value*, Error>
CodeGeneration::getLValueAddress(const AST *lhs) const noexcept {
  // Variable simple
  if (auto varNode = dynamic_cast<const AST_VAR_CALL*>(lhs)) {
    auto varOrErr = currentScope_->getVariable(varNode->id());
    if (!varOrErr) return createError(varOrErr.error());
    return (*varOrErr)->address();
  }

  // Índice de vector
  if (auto idxNode = dynamic_cast<const AST_INDEX*>(lhs)) {
    // calcular índice
    auto idxOrErr = idxNode->index()->accept(*this);
    if (!idxOrErr) return createError(idxOrErr.error());
    llvm::Value *idxVal = *idxOrErr;
    if (idxVal->getType()->isIntegerTy(32))
      idxVal = builder_.CreateSExt(idxVal, builder_.getInt64Ty(), "idx_to_i64");

    // puntero al struct vector
    llvm::Value *basePtr = resultChainedExpression_;
    auto vecType = std::dynamic_pointer_cast<VectorType>(lhs->returnedFromTypeAnalysis());
    auto structTyOrErr = vecType->llvmVersion(context_);
    if (!structTyOrErr) return createError(structTyOrErr.error());
    auto *vecStruct = llvm::cast<llvm::StructType>(*structTyOrErr);

    // GEP al campo .data
    llvm::Value *dataField = builder_.CreateInBoundsGEP(
      vecStruct, basePtr,
      { builder_.getInt64(0), builder_.getInt32(0) },
      "vec_data_gep");

    // load del puntero T*
    auto *ptrElemTy = llvm::cast<llvm::PointerType>(vecStruct->getElementType(0));
    llvm::Value *dataPtr = builder_.CreateLoad(ptrElemTy, dataField, "vec_data_load");

    // GEP al elemento
    auto elemTy = vecType->elementType()->llvmVersion(context_).value();
    return builder_.CreateInBoundsGEP(elemTy, dataPtr, { idxVal }, "vec_elem_gep");
  }

  // AST_CHAINED (p. ej. x.f[3] o simplemente x con cero ops)
  if (auto chain = dynamic_cast<const AST_CHAINED*>(lhs)) {
    // evaluar base y actualizar resultChainedExpression_
    auto baseOrErr = chain->base()->accept(*this);
    if (!baseOrErr) return createError(baseOrErr.error());
    resultChainedExpression_ = *baseOrErr;
    currentType = chain->base()->returnedFromTypeAnalysis();

    // si hay operaciones, delegar la última
    const auto &ops = chain->operations();
    if (!ops.empty()) {
      // aplicar todas menos la última
      for (size_t i = 0; i + 1 < ops.size(); ++i) {
        auto opOrErr = ops[i]->accept(*this);
        if (!opOrErr) return createError(opOrErr.error());
        currentType = ops[i]->returnedFromTypeAnalysis();
      }
      // última operación como LHS
      return getLValueAddress(ops.back().get());
    }

    // sin ops: la base misma es el LHS
    return getLValueAddress(chain->base().get());
  }

  // Fallback: si accept(lhs) devuelve puntero
  if (auto ptrOrErr = lhs->accept(*this);
      ptrOrErr && (*ptrOrErr)->getType()->isPointerTy()) {
    return *ptrOrErr;
  }

  return createError(ERROR_TYPE::BUILDER,
                     "LHS de la asignación no soportado");
}

std::expected<llvm::Value*, Error>
CodeGeneration::visit(const AST_ASSIGNMENT *node) const noexcept {
  if (!node)
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ASSIGNMENT");

  // obtener dirección de LHS
  auto addrOrErr = getLValueAddress(node->left().get());
  if (!addrOrErr) return createError(addrOrErr.error());
  llvm::Value *addr = *addrOrErr;

  // generar valor RHS
  auto valOrErr = node->value()->accept(*this);
  if (!valOrErr) return createError(valOrErr.error());
  llvm::Value *val = *valOrErr;

  // ajustar tipo según semántica
  auto semTy = node->left()->returnedFromTypeAnalysis();
  auto llvmTyOrErr = semTy->llvmVersion(context_);
  if (!llvmTyOrErr) return createError(llvmTyOrErr.error());
  llvm::Type *dstTy = *llvmTyOrErr;
  if (val->getType() != dstTy) {
    if (val->getType()->isIntegerTy() && dstTy->isIntegerTy())
      val = builder_.CreateSExt(val, dstTy, "assign_sext");
    else
      val = builder_.CreateBitCast(val, dstTy, "assign_cast");
  }

  // emitir store
  builder_.CreateStore(val, addr);

  // actualizar para chaining
  resultChainedExpression_ = val;
  currentType = semTy;
  return val;
}


}
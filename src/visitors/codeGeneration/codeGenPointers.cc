#include "../../../inc/parsingAnalysis/ast/pointer/ast_delete.h"
#include "../../../inc/parsingAnalysis/ast/pointer/ast_deref.h"
#include "../../../inc/parsingAnalysis/ast/pointer/ast_new.h"
#include "../../../inc/visitors/codeGeneration/codeGeneration.h"

namespace nicole {

// En codeGeneration.cpp:

void CodeGeneration::ensureMallocFreeDeclared() const noexcept {
  if (!mallocFn_) {
    // void* malloc(size_t)
    auto *i64Ty = llvm::Type::getInt64Ty(context_);
    auto *voidPtr = llvm::Type::getInt8Ty(context_)->getPointerTo();
    auto *mallocFT = llvm::FunctionType::get(voidPtr, {i64Ty}, false);
    mallocFn_ = llvm::cast<llvm::Function>(
      module_->getOrInsertFunction("malloc", mallocFT).getCallee());
  }
  if (!freeFn_) {
    // void free(void*)
    auto *voidPtr = llvm::Type::getInt8Ty(context_)->getPointerTo();
    auto *freeFT = llvm::FunctionType::get(
      llvm::Type::getVoidTy(context_), {voidPtr}, false);
    freeFn_ = llvm::cast<llvm::Function>(
      module_->getOrInsertFunction("free", freeFT).getCallee());
  }
}

std::expected<llvm::Value*, Error>
CodeGeneration::visit(const AST_NEW *node) const noexcept {
  if (!node)
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_NEW");

  // Tipo y número de elementos
  //  Suponemos que node->newType() y node->countExpr() existen
  auto semTy = node->returnedFromTypeAnalysis();
  auto llvmTyOrErr = semTy->llvmVersion(context_);
  if (!llvmTyOrErr) return createError(ERROR_TYPE::TYPE, "no llvm type for new");
  llvm::Type *elemTy = *llvmTyOrErr;

  // Generar el count (número de elementos)
  auto cntOrErr = emitRValue(node->value().get());
  if (!cntOrErr) return createError(cntOrErr.error());
  llvm::Value *countVal = *cntOrErr;

  // Calcular bytes = countVal * sizeof(elemTy)
  const auto &DL = module_->getDataLayout();
  uint64_t sizeOfElem = DL.getTypeAllocSize(elemTy);
  llvm::Value *sizeConst = builder_.getInt64(sizeOfElem);
  llvm::Value *countI64 = builder_.CreateZExtOrBitCast(
      countVal, builder_.getInt64Ty(), "cnt_i64");
  llvm::Value *totalBytes = builder_.CreateMul(
      countI64, sizeConst, "total_bytes");

  // Declarar malloc/free (si es necesario)
  ensureMallocFreeDeclared();

  // Llamada a malloc
  llvm::Value *rawPtr = builder_.CreateCall(
      mallocFn_,
      { totalBytes },
      "new_ptr");

  // Inicializar la memoria a cero
  //  llvm.memset.p0.p0.i64(ptr, i8 0, i64 size, i1 false)
  builder_.CreateMemSet(
      rawPtr,
      builder_.getInt8(0),
      totalBytes,
      llvm::MaybeAlign(1));

  // Registrar el puntero y devolverlo
  allocatedPtrs_.insert(rawPtr);
  return rawPtr;
}


std::expected<llvm::Value*, Error>
CodeGeneration::visit(const AST_DELETE *node) const noexcept {
  if (!node)
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DELETE");

  // Generar el rvalue que es el puntero a liberar
  auto ptrOrErr = emitRValue(node->value().get());
  if (!ptrOrErr) return createError(ptrOrErr.error());
  llvm::Value *ptrVal = *ptrOrErr;

  // Solo liberamos si lo conocemos
  if (allocatedPtrs_.count(ptrVal) == 0) {
    return createError(ERROR_TYPE::TYPE,
      "attempt to delete unallocated pointer");
  }

  // Llamar a free(ptrVal)
  ensureMallocFreeDeclared();
  builder_.CreateCall(freeFn_, { ptrVal });

  // Quitar de la tabla
  allocatedPtrs_.erase(ptrVal);

  // delete no produce valor
  return nullptr;
}

std::expected<llvm::Value*, Error>
CodeGeneration::visit(const AST_DEREF *node) const noexcept {
  if (!node)
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DEREF");

  // Obtener rvalue del subnodo (una dirección)
  auto ptrOrErr = emitRValue(node->value().get());
  if (!ptrOrErr) return createError(ptrOrErr.error());
  llvm::Value *addr = *ptrOrErr;

  // Para dereference en contexto de rvalue, cargamos la dirección:
  //  Si quieres lvalue, usarías emitLValue en vez de accept aquí.
  std::expected<llvm::Type *, Error> pointeeTy = node->returnedFromTypeAnalysis()->llvmVersion(context_);
  if (!pointeeTy) {
    return createError(pointeeTy.error());
  }
  llvm::Value *loaded = builder_.CreateLoad(*pointeeTy, addr, "deref_load");

  return loaded;
}


} // namespace nicole
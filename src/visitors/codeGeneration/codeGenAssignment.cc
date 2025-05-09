#include "../../../inc/visitors/codeGeneration/codeGeneration.h"

#include "../../../inc/parsingAnalysis/ast/assignments/ast_assignment.h"
#include "../../../inc/parsingAnalysis/ast/variables/ast_varCall.h"
#include "../../../inc/parsingAnalysis/ast/vector/ast_index.h"
#include "../../../inc/parsingAnalysis/ast/chained/ast_chained.h"

#include <memory>

namespace nicole {

std::expected<llvm::Value*, Error>
CodeGeneration::visit(const AST_ASSIGNMENT *node) const noexcept {
  if (!node)
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ASSIGNMENT");

  // obtener dirección de LHS
  auto addrOrErr = emitLValue(node->left().get());
  if (!addrOrErr) return createError(addrOrErr.error());
  llvm::Value *addr = *addrOrErr;

  // generar valor RHS
  auto valOrErr = emitRValue(node->value().get());
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
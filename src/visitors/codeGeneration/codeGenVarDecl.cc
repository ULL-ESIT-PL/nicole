#include "../../../inc/parsingAnalysis/ast/variables/ast_autoDecl.h"
#include "../../../inc/parsingAnalysis/ast/variables/ast_typedDecl.h"
#include "../../../inc/parsingAnalysis/ast/variables/ast_varCall.h"
#include "../../../inc/visitors/codeGeneration/codeGeneration.h"

namespace nicole {

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_AUTO_DECL *node) const noexcept {
  if (!node)
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_AUTO_DECL");
  // Generar el valor inicial
  auto initOrErr = node->value()->accept(*this);
  if (!initOrErr)
    return createError(initOrErr.error());
  llvm::Value *initVal = *initOrErr;

  // Recuperar la Variable existente del scope
  auto varOrErr = currentScope_->getVariable(node->id());
  if (!varOrErr)
    return createError(varOrErr.error());
  auto varPtr = *varOrErr; // shared_ptr<Variable>
  Variable &var = *varPtr; // referencia al objeto real

  // Alloca en entry_ usando builder_
  // Guardamos el punto de inserción actual
  llvm::IRBuilder<>::InsertPointGuard guard(builder_);
  // Movemos a justo después de la etiqueta 'entry'
  builder_.SetInsertPoint(entry_, entry_->getFirstInsertionPt());
  auto llvmTyOrErr = var.type()->llvmVersion(context_);
  if (!llvmTyOrErr)
    return std::unexpected(llvmTyOrErr.error());
  llvm::Type *llvmTy = *llvmTyOrErr;
  llvm::AllocaInst *alloca = builder_.CreateAlloca(llvmTy, nullptr, node->id());
  var.setAddress(alloca);
  // Restauramos el punto de inserción original (gracias al guard)

  // Store del valor inicial y actualizar en la Variable
  builder_.CreateStore(initVal, alloca);
  var.setValue(initVal);

  return alloca;
}

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_VAR_TYPED_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_VAR_TYPED_DECL");
  }
  auto initOrErr = node->value()->accept(*this);
  if (!initOrErr)
    return createError(initOrErr.error());
  llvm::Value *initVal = *initOrErr;

  auto varOrErr = currentScope_->getVariable(node->id());
  if (!varOrErr)
    return createError(varOrErr.error());
  auto varPtr = *varOrErr; // shared_ptr<Variable>
  Variable &var = *varPtr; // referencia al objeto real

  llvm::IRBuilder<>::InsertPointGuard guard(builder_);
  builder_.SetInsertPoint(entry_, entry_->getFirstInsertionPt());
  auto llvmTyOrErr = node->varType()->llvmVersion(context_);
  if (!llvmTyOrErr)
    return std::unexpected(llvmTyOrErr.error());
  llvm::Type *llvmTy = *llvmTyOrErr;
  llvm::AllocaInst *alloca = builder_.CreateAlloca(llvmTy, nullptr, node->id());
  var.setAddress(alloca);

  builder_.CreateStore(initVal, alloca);
  var.setValue(initVal);

  return alloca;
}

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_VAR_CALL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_VAR_CALL");
  }
  auto varOrErr = currentScope_->getVariable(node->id());
  if (!varOrErr)
    return createError(varOrErr.error());

  auto varPtr = *varOrErr; // shared_ptr<Variable>
  Variable &var = *varPtr; // referencia al objeto real
  llvm::AllocaInst *addr = var.address();
  if (!addr)
    return createError(ERROR_TYPE::VALIDATE_TREE, "variable has no address");
  // Simplemente load y actualizar el value_
  llvm::Value *loaded =
      builder_.CreateLoad(addr->getAllocatedType(), addr, node->id() + "_val");
  var.setValue(loaded);

  return loaded;
}

} // namespace nicole
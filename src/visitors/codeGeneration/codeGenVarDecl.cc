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
  std::expected<llvm::Value *, Error> initOrErr =
      emitRValue(node->value().get());
  if (!initOrErr)
    return createError(initOrErr.error());
  llvm::Value *initVal = *initOrErr;

  // Recuperar la Variable existente del scope
  std::expected<std::shared_ptr<Variable>, Error> varOrErr =
      currentScope_->getVariable(node->id());
  if (!varOrErr)
    return createError(varOrErr.error());
  std::shared_ptr<Variable> varPtr = *varOrErr; // shared_ptr<Variable>
  Variable &var = *varPtr;                      // referencia al objeto real

  // Alloca en entry_ usando builder_
  // Guardamos el punto de inserción actual
  llvm::IRBuilder<>::InsertPointGuard guard(builder_);
  // Movemos a justo después de la etiqueta 'entry'
  // builder_.SetInsertPoint(entry_, entry_->getFirstInsertionPt());
  // builder_.SetInsertPoint(entry_);
  std::expected<llvm::Type *, Error> llvmTyOrErr =
      var.type()->llvmVersion(context_);
  if (!llvmTyOrErr)
    return std::unexpected(llvmTyOrErr.error());
  llvm::Type *llvmTy = *llvmTyOrErr;
  llvm::AllocaInst *alloca = builder_.CreateAlloca(llvmTy, nullptr, node->id());
  var.setAddress(alloca);
  // Restauramos el punto de inserción original (gracias al guard)

  // Si es un struct (aggregate), primero cargamos el valor y luego lo
  // almacenamos
  if (llvmTy->isAggregateType()) {
    // initVal es un ptr a struct, cargamos el struct completo
    llvm::LoadInst *loadedStruct =
        builder_.CreateLoad(llvmTy, initVal, node->id() + "_agg_load");
    builder_.CreateStore(loadedStruct, alloca);
    var.setValue(loadedStruct);
  } else {
    // Escalar o ptr: almacenar directamente
    builder_.CreateStore(initVal, alloca);
    var.setValue(initVal);
  }

  return alloca;
}

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_VAR_TYPED_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_VAR_TYPED_DECL");
  }
  std::expected<llvm::Value *, Error> initOrErr =
      emitRValue(node->value().get());
  if (!initOrErr)
    return createError(initOrErr.error());
  llvm::Value *initVal = *initOrErr;

  std::expected<std::shared_ptr<Variable>, Error> varOrErr =
      currentScope_->getVariable(node->id());
  if (!varOrErr)
    return createError(varOrErr.error());
  std::shared_ptr<Variable> varPtr = *varOrErr; // shared_ptr<Variable>
  Variable &var = *varPtr;                      // referencia al objeto real

  llvm::IRBuilder<>::InsertPointGuard guard(builder_);
  // builder_.SetInsertPoint(entry_, entry_->getFirstInsertionPt());
  std::expected<llvm::Type *, Error> llvmTyOrErr =
      node->varType()->llvmVersion(context_);
  if (!llvmTyOrErr)
    return std::unexpected(llvmTyOrErr.error());
  llvm::Type *llvmTy = *llvmTyOrErr;
  llvm::AllocaInst *alloca = builder_.CreateAlloca(llvmTy, nullptr, node->id());
  var.setAddress(alloca);

  // Si es un struct (aggregate), primero cargamos el valor y luego lo
  // almacenamos
  if (llvmTy->isAggregateType()) {
    // initVal es un ptr a struct, cargamos el struct completo
    llvm::LoadInst *loadedStruct =
        builder_.CreateLoad(llvmTy, initVal, node->id() + "_agg_load");
    builder_.CreateStore(loadedStruct, alloca);
    var.setValue(loadedStruct);
  } else {
    // Escalar o ptr: almacenar directamente
    builder_.CreateStore(initVal, alloca);
    var.setValue(initVal);
  }

  return alloca;
}

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_VAR_CALL *node) const noexcept {
  if (!node)
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_VAR_CALL");

  std::expected<std::shared_ptr<Variable>, Error> varOrErr =
      currentScope_->getVariable(node->id());
  if (!varOrErr)
    return createError(varOrErr.error());

  std::shared_ptr<Variable> varPtr = *varOrErr;
  llvm::AllocaInst *addr = varPtr->address();
  if (!addr)
    return createError(ERROR_TYPE::VALIDATE_TREE, "variable has no address");

  resultChainedExpression_ = addr;
  return addr;
}

} // namespace nicole
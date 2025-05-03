#include "../../../inc/visitors/codeGeneration/codeGeneration.h"

#include "../../../inc/parsingAnalysis/ast/tree.h"
#include <cstddef>
#include <memory>
#include <variant>

/**

FillSemanticInfo ---> insertar delcaraciones en las tablas / insertar tipos /
comprobar que las variables pertenecen al scope (llamadas a variables) /
comrpobar llamadas a enum

TypeAnalysis ---> comprobar en una llamada a funcion que esta existe debido a
sobrecarga de funciones requiere que se trate en el typeAnalysis / igual con
metodos / llamadas a atributos / variables auto

 */

namespace nicole {

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_STATEMENT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_STATEMENT");
  }
  return node->expression()->accept(*this);
}

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_BODY *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_BODY");
  }
  currentScope_ = node->scope();

  llvm::Value *lastValue{nullptr};

  // Recorremos todas las sentencias del cuerpo
  for (const auto &statement : node->body()) {
    auto res = statement->accept(*this);
    if (!res) {
      // Propagamos el error si alguna sentencia falla
      return createError(res.error());
    }
    // Guardamos el valor devuelto (puede ser nullptr si la sentencia no produce
    // valor)
    lastValue = res.value();
  }

  // Devolvemos el valor de la última sentencia (o nullptr si no había
  // sentencias)
  return lastValue;
}

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const Tree *tree) const noexcept {
  if (!tree) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid Tree");
  }

  funcType_ = llvm::FunctionType::get(
      (options_.validateTree()) ? builder_.getInt32Ty() : builder_.getVoidTy(),
      false);
  mainFunction_ = llvm::Function::Create(
      funcType_, llvm::Function::ExternalLinkage, "main", module_.get());
  entry_ = llvm::BasicBlock::Create(context_, "entry", mainFunction_);
  builder_.SetInsertPoint(entry_);

  const auto result{tree->root()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }

  if (!options_.validateTree()) {
    builder_.CreateRetVoid();
  }

  llvm::verifyModule(*module_, &llvm::errs());

  module_->print(llvm::outs(), nullptr);

  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();
  llvm::InitializeNativeTargetAsmParser();
  std::string errStr;
  llvm::ExecutionEngine *execEngine =
      llvm::EngineBuilder(std::move(module_))
          .setErrorStr(&errStr)
          .setOptLevel(llvm::CodeGenOptLevel::Default)
          .create();
  if (!execEngine) {
    std::cerr << "Failed to create ExecutionEngine: " << errStr << std::endl;
    return nullptr;
  }

  // Ejecuta la función main y obtiene el resultado
  std::vector<llvm::GenericValue> noargs;
  llvm::GenericValue gv = execEngine->runFunction(mainFunction_, noargs);

  delete execEngine;

  return nullptr;
}

} // namespace nicole
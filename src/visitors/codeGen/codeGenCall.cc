#include "../../../inc/visitors/codeGeneration.h"

#include "../../../inc/parsingAnalysis/ast/calls/functionCall.h"
#include "../../../inc/parsingAnalysis/ast/calls/structAcces.h"
#include "../../../inc/parsingAnalysis/ast/calls/structConstructor.h"
#include "../../../inc/parsingAnalysis/ast/calls/variableCall.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/ErrorHandling.h"
#include <cstddef>
#include <ostream>
#include <string>
#include <vector>

namespace nicole {

llvm::Value *CodeGeneration::visit(const NodeStructConstructor *node) const {
  //std::cout << "Typeeeeeeeeees"<< node->table()->areTypesEquivalent(
  //    node->table()->type("pointD")->type(context_),
  //    node->table()->type("str")->type(context_)) << std::flush;
  const auto userType{
      dynamic_cast<const UserType *>(node->table()->type(node->id()).get())};
  llvm::AllocaInst *structAlloc{
      builder_.CreateAlloca(userType->type(context_), nullptr, node->id())};
  std::vector<llvm::Value *> fields{};
  const auto params{node->parameters()};
  for (size_t i{0}; i < params.size(); ++i) {
    fields.push_back(
        builder_.CreateStructGEP(userType->type(context_), structAlloc, i,
                                 userType->attributes()->paramters()[i].first));
  }
  for (size_t i{0}; i < fields.size(); ++i) {
    builder_.CreateStore(params[i]->accept(this), fields[i]);
  }
  return structAlloc;
}

llvm::Value *CodeGeneration::visit(const NodeStructAcces *node) const {
  // std::cout << "---------\n" << *node->table() << std::flush;

  const auto varTable{node->table()};
  //std::cout << "+++++++" << varTable->variableType(node->id())->name() << std::flush;
  auto structType{node->typeTable()
                      ->type(varTable->variableType(node->id())->name())
                      .get()};
  //std::cout << "||||" << structType->name() + " " << std::flush;
  const auto structTypeCasted = dynamic_cast<const UserType *>(structType);
  if (!structTypeCasted) {
    llvm::report_fatal_error("hola");
  }
  const auto index{structTypeCasted->attribute(node->attribute())};
  // Obtener el puntero al objeto de la estructura
  llvm::Value *structPtr =
      builder_.CreateLoad(varTable->variableAddress(node->id())->getType(),
                          varTable->variableAddress(node->id()), node->id());
  // Obtener el puntero al atributo específico dentro de la estructura
  llvm::Value *fieldPtr = builder_.CreateStructGEP(
      structType->type(context_), structPtr, std::get<0>(index), node->attribute());

  // Crear un load para el atributo específico
  llvm::Type *fieldType =
      structType->type(context_)->getStructElementType(std::get<0>(index));
  if (fieldType->isStructTy()) {
    return fieldPtr;
  }
  return builder_.CreateLoad(fieldType, fieldPtr, node->attribute() + "Temp");
}

llvm::Value *CodeGeneration::visit(const NodeVariableCall *node) const {
  std::cout << "---------\n" << *node->table() << std::flush;
  return builder_.CreateLoad(
      node->table()->variableValue(node->id())->getType(),
      node->table()->variableAddress(node->id()), node->id());
}

llvm::Value *CodeGeneration::visit(const NodeFunctionCall *node) const {
  std::cout << "---------\n" << *node->functionTable() << std::flush;
  auto func{node->functionTable()->function(node->id())};
  std::vector<llvm::Value *> args{};
  for (const auto &param : *node) {
    args.push_back(param->accept(this));
  }
  if (func->arg_size() != args.size()) {
    const std::string errStr{
        "Invalid arguments size for function: " + node->id() +
        ", the size is " + std::to_string(func->arg_size())};
    llvm::report_fatal_error(errStr.c_str());
  }
  return builder_.CreateCall(func, args, "call_" + node->id());
}

} // namespace nicole
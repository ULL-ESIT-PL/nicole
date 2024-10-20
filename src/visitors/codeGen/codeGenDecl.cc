#include "../../../inc/visitors/codeGeneration.h"

#include "../../../inc/lexicalAnalysis/type.h"
#include "../../../inc/parsingAnalysis/ast/declaration/constDeclaration.h"
#include "../../../inc/parsingAnalysis/ast/declaration/nodeFunDeclaration.h"
#include "../../../inc/parsingAnalysis/ast/declaration/nodeReturn.h"
#include "../../../inc/parsingAnalysis/ast/declaration/selfAssignment.h"
#include "../../../inc/parsingAnalysis/ast/declaration/structDeclaration.h"
#include "../../../inc/parsingAnalysis/ast/declaration/varDeclaration.h"
#include "../../../inc/parsingAnalysis/ast/declaration/varReassignment.h"
#include "../../../inc/parsingAnalysis/ast/statements/statementList.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/ErrorHandling.h"
#include <cstddef>
#include <ostream>
#include <regex>
#include <string>
#include <vector>

namespace nicole {

llvm::Value *CodeGeneration::visit(const NodeVariableDeclaration *node) const {
  llvm::Value *value{node->expression()->accept(this)};
  llvm::Type *valueType{value->getType()}; // Tipo de la variable
  if (node->varType()->type(context_) == llvm::Type::getVoidTy(*context_)) {
    llvm::report_fatal_error("Cannot assign to type void");
  }
  if (node->varType()->type(context_) != valueType) {
    llvm::report_fatal_error("Type mismatch");
  }
  // Crear la instrucción 'alloca' para reservar espacio para la variable
  llvm::AllocaInst *alloca{
      builder_.CreateAlloca(valueType, nullptr, node->id())};

  // Almacenar el valor en la variable y tambien en la tabla
  builder_.CreateStore(value, alloca);
  const GenericType *varType{node->varType()};
  node->table()->addVariable(node->id(), varType, value, alloca);

  // Devolver el valor almacenado
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeConstDeclaration *node) const {
  llvm::Value *value{node->expression()->accept(this)};
  llvm::Type *valueType{value->getType()}; // Tipo de la variable
  if (node->varType()->type(context_) == llvm::Type::getVoidTy(*context_)) {
    llvm::report_fatal_error("Cannot assign to type void");
  }
  if (node->varType()->type(context_) != valueType) {
    llvm::report_fatal_error("Type mismatch");
  }
  // Crear la instrucción 'alloca' para reservar espacio para la variable
  llvm::AllocaInst *alloca{
      builder_.CreateAlloca(valueType, nullptr, node->id())};

  // Almacenar el valor en la variable y tambien en la tabla
  builder_.CreateStore(value, alloca);
  const GenericType *varType{node->varType()};
  node->table()->addVariable(node->id(), varType, value, alloca, true);
  // Devolver el valor almacenado
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeStructDeclaration *node) const {
  std::string structName = node->structType()->name();
  // Crear una lista de los tipos de los campos
  std::vector<llvm::Type *> fieldTypes;
  // Suponiendo que el cuerpo de la estructura contiene declaraciones de
  // variables
  for (const auto &declaration : *node->body()) {
    // Supongamos que declaration es de tipo NodeVariableDeclaration o similar
    const NodeVariableDeclaration *varDecl =
        dynamic_cast<const NodeVariableDeclaration *>(
            declaration->expression());
    if (varDecl) {
      // Obtén el tipo de la variable
      std::cout << varDecl->varType()->name() << std::flush;
      llvm::Type *fieldType = varDecl->typeTable()
                                  ->type(varDecl->varType()->name())
                                  ->type(context_);
      fieldTypes.push_back(fieldType);
    }
  }
  // Crear el tipo de estructura en LLVM
  llvm::StructType *structType =
      llvm::StructType::create(*context_, fieldTypes, structName);
  // ADD TO TABLE ???

  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeFunctionDeclaration *node) const {
  auto params{node->parameters()};
  std::vector<llvm::Type *> paramTypes{};
  for (const auto &param : *params) {
    paramTypes.push_back(param.second->type(context_));
  }
  llvm::FunctionType *funcType{llvm::FunctionType::get(
      node->returnType()->type(context_), paramTypes, false)};

  llvm::Function *funct{llvm::Function::Create(
      funcType, llvm::Function::ExternalLinkage, node->id(), module_)};
  // adding to the table here before processing body so a recursion call wont
  // conflict whenever it searches inside the table saying it does not exist
  node->functionTable()->addFunction(node->id(), node->returnType(), funct);

  llvm::BasicBlock *entry{llvm::BasicBlock::Create(*context_, "entry", funct)};

  builder_.SetInsertPoint(entry);
  auto paramsVec{params->paramters()};
  for (std::size_t i{0}; i < paramsVec.size(); ++i) {
    auto argument{funct->getArg(i)};
    argument->setName(paramsVec[i].first + std::to_string(i));
    auto type{paramsVec[i].second};
    llvm::AllocaInst *alloca{builder_.CreateAlloca(
        type->type(context_), nullptr, paramsVec[i].first)};
    builder_.CreateStore(argument, alloca);
    node->table()->addVariable(paramsVec[i].first, type.get(), argument,
                               alloca);
  }
  auto value{node->body()->accept(this)};
  if (node->returnType()->type(context_) == llvm::Type::getVoidTy(*context_)) {
    builder_.CreateRetVoid();
  } else {
    bool hasReturn{false};
    for (const auto &inst : *entry) {
      if (llvm::isa<llvm::ReturnInst>(inst)) {
        hasReturn = true;
        break;
      }
    }
    // the code below is meant to detect if a function doesnt have return of if
    // it is inside a else
    llvm::BasicBlock *previousBlock = nullptr;
    // Iterar sobre los bloques básicos en la función
    for (auto &block : *funct) {
      if (block.getName() == funct->back().getName()) {
        // Si hemos llegado al bloque "back", regresamos el bloque anterior
        break; // Regresamos el bloque anterior
      }
      // Actualizamos el bloque anterior
      previousBlock = &block;
    }
    std::regex elseName{"else"};
    if (previousBlock and
        std::regex_match(previousBlock->getName().str(), elseName)) {
      for (const auto &inst : *previousBlock) {
        if (llvm::isa<llvm::ReturnInst>(inst)) {
          hasReturn = true;
          break;
        }
      }
    }
    for (const auto &inst : funct->back()) {
      if (llvm::isa<llvm::ReturnInst>(inst)) {
        hasReturn = true;
        break;
      }
    }
    if (!hasReturn) {
      llvm::report_fatal_error(
          "Non void functions must have a return statement");
    }
  }
  auto mainFun{module_->getFunction("main")};
  builder_.SetInsertPoint(&mainFun->getEntryBlock());
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeVariableReassignment *node) const {
  llvm::AllocaInst *varAddress{node->table()->variableAddress(node->id())};
  llvm::Value *newValue{node->expression()->accept(this)};
  if (node->table()->variableValue(node->id())->getType() !=
      newValue->getType()) {
    llvm::report_fatal_error("Type mismatch at reassignment");
  }
  builder_.CreateStore(newValue, varAddress);
  node->table()->setVariable(node->id(), newValue);

  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeSelfReassignment *node) const {
  llvm::AllocaInst *varAddress{node->table()->variableAddress(node->id())};
  llvm::Value* oldValue{node->table()->variableValue(node->id())};
  llvm::Value *newValue{node->expression()->accept(this)};
  if (node->table()->variableValue(node->id())->getType() !=
      newValue->getType()) {
    llvm::report_fatal_error("Type mismatch at reassignment");
  }
  
  llvm::Value *result{nullptr};
  switch (node->op()) {
  case TokenType::SELF_ADD: {
    if (oldValue->getType()->isFloatingPointTy()) {
      result = builder_.CreateFAdd(oldValue, newValue, "addtmp");
    } else {
      result = builder_.CreateAdd(oldValue, newValue, "addtmp");
    }
    break;
  }
  case TokenType::SELF_SUB: {
    if (oldValue->getType()->isFloatingPointTy()) {
      result = builder_.CreateFSub(oldValue, newValue, "addtmp");
    } else {
      result = builder_.CreateSub(oldValue, newValue, "addtmp");
    }
    break;
  }
  case TokenType::SELF_MULT: {
    if (oldValue->getType()->isFloatingPointTy()) {
      result = builder_.CreateFMul(oldValue, newValue, "addtmp");
    } else {
      result = builder_.CreateMul(oldValue, newValue, "addtmp");
    }
    break;
  }
  case TokenType::SELF_DIV: {
    if (oldValue->getType()->isFloatingPointTy()) {
      result = builder_.CreateFDiv(oldValue, newValue, "addtmp");
    } else {
      result = builder_.CreateSDiv(oldValue, newValue, "addtmp");
    }
    break;
  }
  default: {
    llvm::report_fatal_error("Undefined self asignment");
  }
  }
  builder_.CreateStore(result, varAddress);
  node->table()->setVariable(node->id(), result);

  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeReturn *node) const {
  if (node->isEmptyExpression()) {
    return builder_.CreateRetVoid();
  }
  llvm::Value *result{node->expression()->accept(this)};
  return builder_.CreateRet(result);
}

} // namespace nicole
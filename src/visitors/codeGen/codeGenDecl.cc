#include "../../../inc/visitors/codeGeneration.h"

#include "../../../inc/lexicalAnalysis/type.h"
#include "../../../inc/parsingAnalysis/ast/calls/structConstructor.h"
#include "../../../inc/parsingAnalysis/ast/declaration/autoDeclaration.h"
#include "../../../inc/parsingAnalysis/ast/declaration/constDeclaration.h"
#include "../../../inc/parsingAnalysis/ast/declaration/nodeFunDeclaration.h"
#include "../../../inc/parsingAnalysis/ast/declaration/nodeReturn.h"
#include "../../../inc/parsingAnalysis/ast/declaration/selfAssignment.h"
#include "../../../inc/parsingAnalysis/ast/declaration/structDeclaration.h"
#include "../../../inc/parsingAnalysis/ast/declaration/structSetAttr.h"
#include "../../../inc/parsingAnalysis/ast/declaration/varDeclaration.h"
#include "../../../inc/parsingAnalysis/ast/declaration/varReassignment.h"
#include "../../../inc/parsingAnalysis/ast/statements/statementList.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/ErrorHandling.h"
#include <cstddef>
#include <memory>
#include <ostream>
#include <regex>
#include <string>
#include <vector>

namespace nicole {

llvm::Value *CodeGeneration::visit(const NodeStructSetAttr *node) const {
  std::cout << "---------\n" << *node->table() << std::flush;

  const auto varTable{node->table()};
  if (varTable->isConst(node->id())) {
    llvm::report_fatal_error(
        "Cannot modify de object attributes because the object is const");
  }
  auto structType{node->typeTable()
                      ->type(varTable->variableType(node->id())->name())
                      .get()};
  const auto structTypeCasted = dynamic_cast<const UserType *>(structType);
  const auto index{structTypeCasted->attribute(node->attribute())};

  // Obtener el puntero al objeto de la estructura
  llvm::Value *structPtr =
      builder_.CreateLoad(varTable->variableAddress(node->id())->getType(),
                          varTable->variableAddress(node->id()), node->id());

  // Obtener el puntero al atributo específico dentro de la estructura
  llvm::Value *fieldPtr = builder_.CreateStructGEP(
      structType->type(context_), structPtr, index.first, node->attribute());

  // Crear un load para el atributo específico
  llvm::Type *fieldType =
      structType->type(context_)->getStructElementType(index.first);

  llvm::Value *expression = node->value()->accept(this);
  builder_.CreateStore(expression, fieldPtr);
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeVariableDeclaration *node) const {
  llvm::Value *value{node->expression()->accept(this)};
  llvm::Type *valueType{value->getType()}; // Tipo de la variable
  auto varType{node->typeTable()->type(node->varType())};
  if (varType->type(context_) == llvm::Type::getVoidTy(*context_)) {
    llvm::report_fatal_error("Cannot assign to type void");
  }
  bool isStruct{false};
  // Si valueType es un puntero, puede ser el constructor de un struct
  if (valueType->isPointerTy()) {
    // convierto el tipo de la variable para saber si valueType es un struct
    llvm::PointerType *expectedType =
        llvm::PointerType::get(varType->type(context_), 0);
    if (expectedType == valueType) {
      isStruct = true;
    }
  }

  // Comparamos el tipo base en vez del tipo puntero
  if (!isStruct && varType->type(context_) != valueType) {
    auto left = varType->type(context_);
    std::string typeStr;
    llvm::raw_string_ostream rso(typeStr);
    left->print(rso);
    std::cout << "El tipo de left es: " << rso.str() << std::endl;

    // Imprimir el tipo de valueType después de desreferenciar
    valueType->print(rso);
    std::cout << "El tipo de valueType es: " << rso.str() << std::endl;

    llvm::report_fatal_error("Type mismatch");
  }

  // Crear la instrucción 'alloca' para reservar espacio para la variable
  llvm::AllocaInst *alloca{
      builder_.CreateAlloca(valueType, nullptr, node->id())};

  // Almacenar el valor en la variable y tambien en la tabla
  builder_.CreateStore(value, alloca);
  const GenericType *type{varType.get()};
  node->table()->addVariable(node->id(), type, value, alloca);

  // Devolver el valor almacenado
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeAutoDeclaration *node) const {
  llvm::Value *value{node->expression()->accept(this)};
  llvm::Type *valueType{value->getType()}; // Tipo de la variable
  llvm::MDNode *meta = llvm::MDNode::get(*context_, llvm::MDString::get(*context_, "mi_metadata"));
  // if it is a constructor we cant comapre llvm::PointerType so we need to infer the type from our type table
  if (node->expression()->type() == NodeType::CALL_CTR) {
    auto casted{dynamic_cast<const NodeStructConstructor*>(node->expression())};
    valueType = casted->table()->type(casted->id())->type(context_);
  }
  std::shared_ptr<GenericType> varType{node->typeTable()->keyFromLLVMType(valueType, *context_)};
  if (!valueType) {
    llvm::report_fatal_error("valuetype is null");
  }
  if (!node->typeTable()->llvmTypeExist(valueType, context_)) {
    llvm::report_fatal_error("LLvm type does not exist in the type table");
  }
  if (valueType == llvm::Type::getVoidTy(*context_)) {
    llvm::report_fatal_error("Cannot assign to type void");
  }

  if (varType->type(context_) == llvm::Type::getVoidTy(*context_)) {
    llvm::report_fatal_error("Cannot assign to type void");
  }
  bool isStruct{false};
  // Si valueType es un puntero, puede ser el constructor de un struct
  if (valueType->isPointerTy()) {
    // convierto el tipo de la variable para saber si valueType es un struct
    llvm::PointerType *expectedType =
        llvm::PointerType::get(varType->type(context_), 0);
    if (expectedType == valueType) {
      isStruct = true;
    }
  }

  // Comparamos el tipo base en vez del tipo puntero
  if (!isStruct && varType->type(context_) != valueType) {
    auto left = varType->type(context_);
    std::string typeStr;
    llvm::raw_string_ostream rso(typeStr);
    left->print(rso);
    std::cout << "El tipo de left es: " << rso.str() << std::endl;

    // Imprimir el tipo de valueType después de desreferenciar
    valueType->print(rso);
    std::cout << "El tipo de valueType es: " << rso.str() << std::endl;

    llvm::report_fatal_error("Type mismatch");
  }
  // Crear la instrucción 'alloca' para reservar espacio para la variable
  llvm::AllocaInst *alloca{
      builder_.CreateAlloca(valueType, nullptr, node->id())};

  // Almacenar el valor en la variable y tambien en la tabla
  builder_.CreateStore(value, alloca);
  
  // std::cout << varType->name() << std::flush;
  std::cout << node->id() << " " << varType->name();
  node->table()->addVariable(node->id(), varType.get(), value, alloca);
  // Devolver el valor almacenado
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeConstDeclaration *node) const {
  llvm::Value *value{node->expression()->accept(this)};
  llvm::Type *valueType{value->getType()}; // Tipo de la variable
  auto varType{node->typeTable()->type(node->varType())};
  if (varType->type(context_) == llvm::Type::getVoidTy(*context_)) {
    llvm::report_fatal_error("Cannot assign to type void");
  }
  bool isStruct{false};
  // Si valueType es un puntero, puede ser el constructor de un struct
  if (valueType->isPointerTy()) {
    // convierto el tipo de la variable para saber si valueType es un struct
    llvm::PointerType *expectedType =
        llvm::PointerType::get(varType->type(context_), 0);
    if (expectedType == valueType) {
      isStruct = true;
    }
  }

  // Comparamos el tipo base en vez del tipo puntero
  if (!isStruct && varType->type(context_) != valueType) {
    auto left = varType->type(context_);
    std::string typeStr;
    llvm::raw_string_ostream rso(typeStr);
    left->print(rso);
    std::cout << "El tipo de left es: " << rso.str() << std::endl;

    // Imprimir el tipo de valueType después de desreferenciar
    valueType->print(rso);
    std::cout << "El tipo de valueType es: " << rso.str() << std::endl;

    llvm::report_fatal_error("Type mismatch");
  }
  // Crear la instrucción 'alloca' para reservar espacio para la variable
  llvm::AllocaInst *alloca{
      builder_.CreateAlloca(valueType, nullptr, node->id())};

  // Almacenar el valor en la variable y tambien en la tabla
  builder_.CreateStore(value, alloca);
  const GenericType *type{varType.get()};
  node->table()->addVariable(node->id(), type, value, alloca, true);
  // Devolver el valor almacenado
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeStructDeclaration *node) const {
  std::string structName = node->name();
  // Crear una lista de los tipos de los campos
  std::vector<llvm::Type *> fieldTypes;
  // Suponiendo que el cuerpo de la estructura contiene declaraciones de
  // variables
  for (const auto &declaration : *node->attributes()) {
    // Obtén el tipo de la variable
    // std::cout << declaration.first << std::flush;
    auto type{node->typeTable()->type(declaration.second)};
    llvm::Type *fieldType = type->type(context_);
    fieldTypes.push_back(fieldType);
  }
  // Crear el tipo de estructura en LLVM
  llvm::StructType *structType =
      llvm::StructType::create(*context_, fieldTypes, structName);
  // ADD TO TABLE ???
  auto userType{std::make_shared<UserType>(structName)};
  userType->setAttributes(node->attributes());
  node->typeTable()->addType(userType);
  
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeFunctionDeclaration *node) const {
  auto params{node->parameters()};
  std::vector<llvm::Type *> paramTypes{};
  for (const auto &param : *params) {
    paramTypes.push_back(node->typeTable()->type(param.second)->type(context_));
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
    auto typeStr{paramsVec[i].second};
    auto type{node->typeTable()->type(typeStr)};
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
  llvm::Value *oldValue{node->table()->variableValue(node->id())};
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
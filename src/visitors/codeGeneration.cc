#include "../../inc/visitors/codeGeneration.h"

#include "../../inc/lexicalAnalysis/type.h"
#include "../../inc/parsingAnalysis/ast/calls/functionCall.h"
#include "../../inc/parsingAnalysis/ast/calls/structConstructor.h"
#include "../../inc/parsingAnalysis/ast/calls/variableCall.h"
#include "../../inc/parsingAnalysis/ast/conditionals/nodeIfStatement.h"
#include "../../inc/parsingAnalysis/ast/declaration/constDeclaration.h"
#include "../../inc/parsingAnalysis/ast/declaration/nodeFunDeclaration.h"
#include "../../inc/parsingAnalysis/ast/declaration/nodeReturn.h"
#include "../../inc/parsingAnalysis/ast/declaration/structDeclaration.h"
#include "../../inc/parsingAnalysis/ast/declaration/varDeclaration.h"
#include "../../inc/parsingAnalysis/ast/declaration/varReassignment.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralBool.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralChar.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralDouble.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralFloat.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralInt.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralString.h"
#include "../../inc/parsingAnalysis/ast/loops/nodeForStatement.h"
#include "../../inc/parsingAnalysis/ast/loops/nodePass.h"
#include "../../inc/parsingAnalysis/ast/loops/nodeStop.h"
#include "../../inc/parsingAnalysis/ast/loops/nodeWhileStatement.h"
#include "../../inc/parsingAnalysis/ast/operations/nodeBinaryOp.h"
#include "../../inc/parsingAnalysis/ast/operations/nodeIncrement.h"
#include "../../inc/parsingAnalysis/ast/operations/nodeUnaryOp.h"
#include "../../inc/parsingAnalysis/ast/statements/statement.h"
#include "../../inc/parsingAnalysis/ast/statements/statementList.h"
#include "../../inc/parsingAnalysis/ast/utils/nodePrint.h"
#include "../../inc/parsingAnalysis/parsingAlgorithms/tree.h"
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

llvm::Value *CodeGeneration::visit(const NodeLiteralBool *node) const {
  return llvm::ConstantInt::get(llvm::Type::getInt1Ty(*context_),
                                node->value());
}

llvm::Value *CodeGeneration::visit(const NodeLiteralChar *node) const {
  return llvm::ConstantInt::get(llvm::Type::getInt8Ty(*context_),
                                node->value());
}

llvm::Value *CodeGeneration::visit(const NodeLiteralFloat *node) const {
  return llvm::ConstantFP::get(llvm::Type::getFloatTy(*context_),
                               llvm::APFloat(node->value()));
}

llvm::Value *CodeGeneration::visit(const NodeLiteralDouble *node) const {
  return llvm::ConstantFP::get(llvm::Type::getDoubleTy(*context_),
                               llvm::APFloat(node->value()));
}

llvm::Value *CodeGeneration::visit(const NodeLiteralInt *node) const {
  return llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context_),
                                node->value(), true);
}

llvm::Value *CodeGeneration::visit(const NodeLiteralString *node) const {
  llvm::Constant *strConst = llvm::ConstantDataArray::getString(
      *context_, node->value(), /*AddNull=*/true);

  llvm::Value *globalString{
      builder_.CreateGlobalString(llvm::StringRef{node->value()}, "str", 0U)};
  // Obtener el puntero al string global
  llvm::Value *globalStrPtr = builder_.CreatePointerCast(
      globalString, llvm::PointerType::getUnqual(strConst->getType()));

  return globalStrPtr;
}

llvm::Value *CodeGeneration::visit(const NodeReturn *node) const {
  if (node->isEmptyExpression()) {
    return builder_.CreateRetVoid();
  }
  llvm::Value *result{node->expression()->accept(this)};
  return builder_.CreateRet(result);
}

llvm::Value *CodeGeneration::visit(const NodeBinaryOp *node) const {
  const Node *left{node->left()};
  const Node *right{node->right()};

  llvm::Value *leftEvaluated{left->accept(this)};
  llvm::Value *rightEvaluated{right->accept(this)};
  if (!leftEvaluated || !rightEvaluated) {
    return nullptr;
  }
  // Convertir tipos si son diferentes (int a double)
  llvm::Type *leftType = leftEvaluated->getType();
  llvm::Type *rightType = rightEvaluated->getType();

  // Si left es int y right es double, convierte left a double
  if (leftType->isIntegerTy() && rightType->isFloatingPointTy()) {
    leftEvaluated =
        builder_.CreateSIToFP(leftEvaluated, rightType, "intToDouble");
  }
  // Si left es double y right es int, convierte right a double
  else if (leftType->isFloatingPointTy() && rightType->isIntegerTy()) {
    rightEvaluated =
        builder_.CreateSIToFP(rightEvaluated, leftType, "intToDouble");
  }

  if (leftEvaluated->getType() != rightEvaluated->getType()) {
    return nullptr; // Error: tipos no compatibles
  }
  // Realiza la operación basada en el tipo del operador
  switch (node->op()) {
  case TokenType::OPERATOR_ADD:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder_.CreateFAdd(leftEvaluated, rightEvaluated, "addtmp");
    } else {
      return builder_.CreateAdd(leftEvaluated, rightEvaluated, "addtmp");
    }
  case TokenType::OPERATOR_SUB:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder_.CreateFSub(leftEvaluated, rightEvaluated, "subtmp");
    } else {
      return builder_.CreateSub(leftEvaluated, rightEvaluated, "subtmp");
    }
  case TokenType::OPERATOR_MULT:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder_.CreateFMul(leftEvaluated, rightEvaluated, "multmp");
    } else {
      return builder_.CreateMul(leftEvaluated, rightEvaluated, "multmp");
    }
  case TokenType::OPERATOR_DIV:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder_.CreateFDiv(leftEvaluated, rightEvaluated, "divmp");
    } else {
      return builder_.CreateSDiv(leftEvaluated, rightEvaluated, "divtmp");
    }
  case TokenType::OPERATOR_MODULE:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      llvm::report_fatal_error(
          "Cannot calculate the module with decimal numbers");
    } else {
      return builder_.CreateURem(leftEvaluated, rightEvaluated, "modtmp");
    }
  case TokenType::OR:
    if (leftEvaluated->getType()->isIntegerTy()) {
      return builder_.CreateOr(leftEvaluated, rightEvaluated, "ormp");
    }
  case TokenType::AND:
    if (leftEvaluated->getType()->isIntegerTy()) {
      return builder_.CreateAnd(leftEvaluated, rightEvaluated, "andmp");
    }
  case TokenType::EQUAL:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder_.CreateFCmpOEQ(leftEvaluated, rightEvaluated, "equalmp");
    } else {
      return builder_.CreateICmpEQ(leftEvaluated, rightEvaluated, "equaltmp");
    }
  case TokenType::NOTEQUAL:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder_.CreateFCmpONE(leftEvaluated, rightEvaluated,
                                    "notequalmp");
    } else {
      return builder_.CreateICmpNE(leftEvaluated, rightEvaluated,
                                   "notequaltmp");
    }
  case TokenType::OPERATOR_SMALLER:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder_.CreateFCmpOLT(leftEvaluated, rightEvaluated, "smallermp");
    } else {
      return builder_.CreateICmpSLT(leftEvaluated, rightEvaluated,
                                    "smallertmp");
    }
  case TokenType::SMALLEREQUAL:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder_.CreateFCmpOLE(leftEvaluated, rightEvaluated,
                                    "smallerequalmp");
    } else {
      return builder_.CreateICmpSLE(leftEvaluated, rightEvaluated,
                                    "smallerequaltmp");
    }
  case TokenType::OPERATOR_GREATER:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder_.CreateFCmpOGT(leftEvaluated, rightEvaluated, "greatermp");
    } else {
      return builder_.CreateICmpSGT(leftEvaluated, rightEvaluated,
                                    "greatertmp");
    }
  case TokenType::BIGGEREQUAL:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder_.CreateFCmpOGE(leftEvaluated, rightEvaluated,
                                    "biggerequalmp");
    } else {
      return builder_.CreateICmpSGE(leftEvaluated, rightEvaluated,
                                    "biggerequaltmp");
    }
  default:
    return nullptr;
  }
}

llvm::Value *CodeGeneration::visit(const NodeUnaryOp *node) const {
  const Node *expression{node->expression()};

  llvm::Value *expressionEvaluated{expression->accept(this)};
  if (!expressionEvaluated) {
    return nullptr;
  }
  llvm::Type *expressionType = expressionEvaluated->getType();
  switch (node->op()) {
  case TokenType::OPERATOR_NOT: {
    if (expressionType->isIntegerTy(1)) {
      return builder_.CreateNot(expressionEvaluated, "notTemp");
    } else {
      llvm::report_fatal_error("Can only use not operator with booleans");
    }
  }
  case TokenType::OPERATOR_SUB: {
    if (expressionType->isIntegerTy()) {
      return builder_.CreateNeg(expressionEvaluated, "arithNegTemp");
    } else if (expressionType->isDoubleTy()) {
      return builder_.CreateFNeg(expressionEvaluated, "arithNegTemp");
    } else {
      llvm::report_fatal_error(
          "Can only use arithmetic negation operator with int or double");
    }
  }
  default:
    llvm::llvm_unreachable_internal("Operator not supported");
  }
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeIncrement *node) const {
  const Node *expression{node->expression()};
  if (expression->type() != NodeType::CALL_VAR) {
    llvm::report_fatal_error(
        "Can only use ++ operator with variables of type int");
  }
  const NodeVariableCall *castedExpression{
      dynamic_cast<const NodeVariableCall *>(expression)};
  llvm::Value *expressionEvaluated{expression->accept(this)};
  if (!expressionEvaluated) {
    return nullptr;
  }
  llvm::Type *expressionType = expressionEvaluated->getType();
  if (!expressionType->isIntegerTy(32)) {
    llvm::report_fatal_error(
        "Can only use ++ operator with variables of type int");
  }
  llvm::Value *one{
      llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context_), 1)};
  switch (node->op()) {
  case TokenType::INCREMENT: {
    llvm::Value *newValue{
        builder_.CreateAdd(expressionEvaluated, one, "increTemp")};
    llvm::AllocaInst *varAddress{
        castedExpression->table()->variableAddress(castedExpression->id())};
    builder_.CreateStore(newValue, varAddress);
    castedExpression->table()->setVariable(castedExpression->id(), newValue);
    return newValue;
  }
  case TokenType::DECREMENT: {
    llvm::Value *newValue{
        builder_.CreateSub(expressionEvaluated, one, "decreTemp")};
    llvm::AllocaInst *varAddress{
        castedExpression->table()->variableAddress(castedExpression->id())};
    builder_.CreateStore(newValue, varAddress);
    castedExpression->table()->setVariable(castedExpression->id(), newValue);
    return newValue;
  }
  default:
    llvm::llvm_unreachable_internal("Operator not supported");
  }
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeStatement *node) const {
  return node->expression()->accept(this);
}

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

llvm::Value *CodeGeneration::visit(const NodeStructConstructor *node) const {
  llvm::report_fatal_error("hola");
  return nullptr;
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

llvm::Value *CodeGeneration::visit(const NodeIfStatement *node) const {
  llvm::Value *condition = node->condition()->accept(this);
  if (!condition) {
    return nullptr;
  }

  llvm::Function *TheFunction = builder_.GetInsertBlock()->getParent();
  llvm::BasicBlock *ThenBB =
      llvm::BasicBlock::Create(*context_, "then", TheFunction);
  llvm::BasicBlock *ElseBB = nullptr;
  llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(*context_, "ifcont");

  if (node->hasElse()) {
    // Crear la instrucción condicional con 'then' y 'else'
    ElseBB = llvm::BasicBlock::Create(*context_, "else");
    builder_.CreateCondBr(condition, ThenBB, ElseBB);
  } else {
    // Crear la instrucción condicional con solo 'then'
    builder_.CreateCondBr(condition, ThenBB, MergeBB);
  }

  // Insertar el bloque 'then'
  builder_.SetInsertPoint(ThenBB);
  node->body()->accept(this); // Ejecutar el cuerpo del 'then'
  // to avoid branching if has return
  bool ifHasReturn{false};
  for (const auto &inst : *ThenBB) {
    if (llvm::isa<llvm::ReturnInst>(inst)) {
      ifHasReturn = true;
    }
  }
  if (!ifHasReturn) {
    builder_.CreateBr(MergeBB); // Unir con MergeBB
  }
  if (ElseBB) {
    // Insertar el bloque 'else'
    TheFunction->insert(TheFunction->end(), ElseBB);
    builder_.SetInsertPoint(ElseBB);
    node->elseBody()->accept(this); // Ejecutar el cuerpo del 'else'
    // to avoid branching if has return
    bool elseHasReturn{false};
    for (const auto &inst : *ElseBB) {
      if (llvm::isa<llvm::ReturnInst>(inst)) {
        elseHasReturn = true;
      }
    }
    if (!elseHasReturn) {
      builder_.CreateBr(MergeBB); // Unir con MergeBB
    }
  }

  // Insertar el bloque 'merge'
  TheFunction->insert(TheFunction->end(), MergeBB);
  builder_.SetInsertPoint(MergeBB);
  // No devolver valor ya que el 'if' solo controla el flujo
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeWhileStatement *node) const {
  llvm::Function *TheFunction = builder_.GetInsertBlock()->getParent();

  llvm::BasicBlock *CondBB =
      llvm::BasicBlock::Create(*context_, "while.cond", TheFunction);
  llvm::BasicBlock *BodyBB = llvm::BasicBlock::Create(
      *context_, "while.body", TheFunction); // si pongo Thefunction luego no me
                                             // hace falta hacer un insert
  llvm::BasicBlock *EndBB = llvm::BasicBlock::Create(*context_, "while.end");

  // Saltar inmediatamente a la condición del bucle
  builder_.CreateBr(CondBB);

  // Insertar el bloque de condición
  builder_.SetInsertPoint(CondBB);

  // Evaluar la condición
  llvm::Value *condition = node->condition()->accept(this);
  if (!condition) {
    return nullptr; // Error al generar la condición
  }

  // Crear una instrucción condicional para decidir si continuar con el bucle o
  // terminar
  builder_.CreateCondBr(condition, BodyBB, EndBB);

  // Insertar el bloque del cuerpo del bucle
  builder_.SetInsertPoint(BodyBB);

  // Generar el cuerpo del bucle
  node->body()->accept(this);

  // Al finalizar el cuerpo, saltar nuevamente a la condición del bucle
  builder_.CreateBr(CondBB);

  // Insertar el bloque de finalización (salida del bucle)
  TheFunction->insert(TheFunction->end(), EndBB);
  builder_.SetInsertPoint(EndBB);

  // No retorna ningún valor
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeForStatement *node) const {
  // Crear los bloques básicos: condición, cuerpo y finalización del while

  auto initiation{node->init()->accept(this)};

  llvm::Function *TheFunction = builder_.GetInsertBlock()->getParent();

  llvm::BasicBlock *CondBB =
      llvm::BasicBlock::Create(*context_, "for.cond", TheFunction);
  llvm::BasicBlock *BodyBB = llvm::BasicBlock::Create(
      *context_, "for.body", TheFunction); // si pongo Thefunction luego no me
                                           // hace falta hacer un insert
  llvm::BasicBlock *EndBB = llvm::BasicBlock::Create(*context_, "for.end");

  // Saltar inmediatamente a la condición del bucle
  builder_.CreateBr(CondBB);

  // Insertar el bloque de condición
  builder_.SetInsertPoint(CondBB);

  // Evaluar la condición
  llvm::Value *condition = node->condition()->accept(this);
  if (!condition) {
    return nullptr; // Error al generar la condición
  }

  // Crear una instrucción condicional para decidir si continuar con el bucle o
  // terminar
  builder_.CreateCondBr(condition, BodyBB, EndBB);

  // Insertar el bloque del cuerpo del bucle
  builder_.SetInsertPoint(BodyBB);

  // Generar el cuerpo del bucle
  node->body()->accept(this);
  node->update()->accept(this);

  // Al finalizar el cuerpo, saltar nuevamente a la condición del bucle
  builder_.CreateBr(CondBB);

  // Insertar el bloque de finalización (salida del bucle)
  TheFunction->insert(TheFunction->end(), EndBB);
  builder_.SetInsertPoint(EndBB);

  // No retorna ningún valor
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeStop *node) const {
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodePass *node) const {
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeStatementList *node) const {
  llvm::Value *lastValue{nullptr};
  for (const auto &statement : *node) {
    llvm::Value *value{statement->accept(this)};
    if (statement->expression()->type() == NodeType::VAR_DECL ||
        statement->expression()->type() == NodeType::CONST_DECL ||
        statement->expression()->type() == NodeType::VAR_REG ||
        statement->expression()->type() == NodeType::IF ||
        statement->expression()->type() == NodeType::WHILE ||
        statement->expression()->type() == NodeType::FOR ||
        statement->expression()->type() == NodeType::STOP ||
        statement->expression()->type() == NodeType::PASS ||
        statement->expression()->type() == NodeType::STRUCT_DECL ||
        statement->expression()->type() == NodeType::FUN_DECL ||
        statement->expression()->type() == NodeType::PRINT) {
      // std::cout << "SKIPPED->>>"
      //         << nodeTypeToString(statement->expression()->type()) + "\n"
      //       << std::flush;
      continue;
    }
    if (!value) {
      return nullptr;
    }
    // std::cout << "NOT SKIPPED->>>"
    //       << nodeTypeToString(statement->expression()->type()) + "\n"
    //     << std::flush;
    lastValue = value;
  }

  return lastValue;
}

llvm::Value *CodeGeneration::visit(const NodePrint *node) const {
  std::vector<llvm::Value *> values{};
  std::string fullFormatStr = "";

  for (const auto &expr : node->expressions()) {
    auto value = expr->accept(this);
    if (!value) {
      llvm::report_fatal_error("Expression evaluation failed.");
    }
    values.push_back(value);
  }

  auto paramsAndFormats{printParameters(values, context_, builder_)};
  std::vector<llvm::Value *> args{};

  for (const auto &paramAndFormat : paramsAndFormats) {
    fullFormatStr += paramAndFormat.second; // Concatenar el formato
    args.push_back(paramAndFormat.first);   // Añadir el valor
  }

  // Check if printf already exists in the module
  llvm::Function *printfFunc = module_->getFunction("printf");
  if (!printfFunc) {
    llvm::FunctionType *printfType = llvm::FunctionType::get(
        llvm::IntegerType::getInt32Ty(*context_),
        llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(*context_)), true);
    printfFunc = llvm::Function::Create(
        printfType, llvm::Function::ExternalLinkage, "printf", module_);
  }

  // Crear una cadena de formato y pasarla junto con los valores a printf
  llvm::Value *formatStr = builder_.CreateGlobalStringPtr(fullFormatStr, "fmt");
  args.insert(args.begin(),
              formatStr); // Insert the format at the beginning of the arguments

  // call printf
  builder_.CreateCall(printfFunc, args, "calltmp");

  return nullptr;
}

llvm::Value *CodeGeneration::visit(const Tree *node) const {
  llvm::Value *val{node->root()->accept(this)};
  /*
    llvm::StructType *pointType =
        llvm::StructType::getTypeByName(*context_, "point");
    llvm::Constant *xValue =
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context_), 10); // x = 10
    llvm::Constant *yValue =
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context_), 20); // y = 20

    // Crear una constante de estructura con los valores de `x` e `y`
    llvm::Constant *pointInstance =
        llvm::ConstantStruct::get(pointType, {xValue, yValue});

    // Crear una variable local para `point`
    llvm::AllocaInst *pointVar =
        builder_.CreateAlloca(pointType, nullptr, "pointVar");

    // Almacenar la instancia en `pointVar`
    builder_.CreateStore(pointInstance, pointVar);

    // Acceder al campo `y`
    // Primero, cargamos la estructura desde `pointVar`
    llvm::LoadInst *loadedPoint = builder_.CreateLoad(pointType, pointVar,
    "loadedPoint");

    // Extraer el campo `y`, que se supone es el índice 1 en la estructura
    llvm::Value *yField = builder_.CreateExtractValue(loadedPoint, 1, "yField");

    // Reasignar `y` a un nuevo valor (40)
    llvm::Constant *newYValue =
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context_), 40);

    // Crear una nueva instancia de la estructura con el nuevo valor de `y`
    llvm::Value *newPoint =
        llvm::ConstantStruct::get(pointType, {xValue, newYValue});

    // Almacenar la nueva estructura de vuelta en `pointVar`
    builder_.CreateStore(newPoint, pointVar);
    */
  return builder_.CreateRetVoid();
}

llvm::Value *CodeGeneration::generate(const Tree *tr) const {
  // std::cout << "Generate--> " << builder_.GetInsertBlock()->getName().str()
  //         << "\n";
  auto val{visit(tr)};
  // std::cout << "Generate--> " << builder_.GetInsertBlock()->getName().str()
  //       << "\n";
  return val;
}
} // namespace nicole

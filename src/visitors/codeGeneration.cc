#include "../../inc/visitors/codeGeneration.h"

#include "../../inc/lexicalAnalysis/type.h"
#include "../../inc/parsingAnalysis/ast/calls/variableCall.h"
#include "../../inc/parsingAnalysis/ast/conditionals/NodeIfStatetement.h"
#include "../../inc/parsingAnalysis/ast/declaration/varDeclaration.h"
#include "../../inc/parsingAnalysis/ast/declaration/varReassignment.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralBool.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralChar.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralDouble.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralInt.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralString.h"
#include "../../inc/parsingAnalysis/ast/operations/nodeBinaryOp.h"
#include "../../inc/parsingAnalysis/ast/statements/statement.h"
#include "../../inc/parsingAnalysis/ast/statements/statementList.h"
#include "../../inc/parsingAnalysis/parsingAlgorithms/tree.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/ErrorHandling.h"

namespace nicole {

llvm::Value *CodeGeneration::visit(const NodeLiteralBool *node) const {
  return llvm::ConstantInt::get(llvm::Type::getInt1Ty(*context_),
                                node->value());
}

llvm::Value *CodeGeneration::visit(const NodeLiteralChar *node) const {
  return llvm::ConstantInt::get(llvm::Type::getInt8Ty(*context_),
                                node->value());
}

llvm::Value *CodeGeneration::visit(const NodeLiteralDouble *node) const {
  return llvm::ConstantFP::get(*context_, llvm::APFloat(node->value()));
}

llvm::Value *CodeGeneration::visit(const NodeLiteralInt *node) const {
  return llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context_),
                                node->value(), true);
}

llvm::Value *CodeGeneration::visit(const NodeLiteralString *node) const {
  llvm::IRBuilder<> builder{*context_};
  llvm::Constant *strConst = llvm::ConstantDataArray::getString(
      *context_, node->value(), /*AddNull=*/true);

  llvm::Value *globalString{builder.CreateGlobalString(
      llvm::StringRef{node->value()}, "str", 0U, module_)};
  // Obtener el puntero al string global
  llvm::Value *globalStrPtr = builder.CreatePointerCast(
      globalString, llvm::PointerType::getUnqual(strConst->getType()));

  return globalStrPtr;
}

llvm::Value *CodeGeneration::visit(const NodeBinaryOp *node) const {
  Node *left{node->left()};
  Node *right{node->right()};

  llvm::Value *leftEvaluated{left->accept(this)};
  llvm::Value *rightEvaluated{right->accept(this)};
  if (!leftEvaluated || !rightEvaluated) {
    return nullptr;
  }
  llvm::IRBuilder<> builder(*context_);
  // Convertir tipos si son diferentes (int a double)
  llvm::Type *leftType = leftEvaluated->getType();
  llvm::Type *rightType = rightEvaluated->getType();

  // Si left es int y right es double, convierte left a double
  if (leftType->isIntegerTy() && rightType->isFloatingPointTy()) {
    leftEvaluated =
        builder.CreateSIToFP(leftEvaluated, rightType, "intToDouble");
  }
  // Si left es double y right es int, convierte right a double
  else if (leftType->isFloatingPointTy() && rightType->isIntegerTy()) {
    rightEvaluated =
        builder.CreateSIToFP(rightEvaluated, leftType, "intToDouble");
  }

  // Asegúrate de que ambos operandos tengan el mismo tipo ahora (ambos double o
  // ambos int)
  if (leftEvaluated->getType() != rightEvaluated->getType()) {
    return nullptr; // Error: tipos no compatibles
  }
  // Realiza la operación basada en el tipo del operador
  switch (node->op()) {
  case TokenType::OPERATOR_ADD:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder.CreateFAdd(leftEvaluated, rightEvaluated, "addtmp");
    } else {
      return builder.CreateAdd(leftEvaluated, rightEvaluated, "addtmp");
    }
  case TokenType::OPERATOR_SUB:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder.CreateFSub(leftEvaluated, rightEvaluated, "subtmp");
    } else {
      return builder.CreateSub(leftEvaluated, rightEvaluated, "subtmp");
    }
  case TokenType::OPERATOR_MULT:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder.CreateFMul(leftEvaluated, rightEvaluated, "multmp");
    } else {
      return builder.CreateMul(leftEvaluated, rightEvaluated, "multmp");
    }
  case TokenType::OPERATOR_DIV:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder.CreateFDiv(leftEvaluated, rightEvaluated, "divmp");
    } else {
      return builder.CreateSDiv(leftEvaluated, rightEvaluated, "divtmp");
    }
  default:
    return nullptr;
  }
}

llvm::Value *CodeGeneration::visit(const NodeStatement *node) const {
  // not using expression leads to infinite loop
  return node->expression()->accept(this);
}

llvm::Value *CodeGeneration::visit(const NodeVariableDeclaration *node) const {
  llvm::IRBuilder<> builder{entry_}; // Obtener el contexto del módulo

  llvm::Value *value{node->expression()->accept(this)};
  llvm::Type *valueType{value->getType()}; // Tipo de la variable

  // Crear la instrucción 'alloca' para reservar espacio para la variable
  llvm::AllocaInst *alloca{
      builder.CreateAlloca(valueType, nullptr, node->id())};

  // Almacenar el valor en la variable y tambien en la tabla
  builder.CreateStore(value, alloca);
  std::unique_ptr<GenericType> varType(std::move(node->varType()));
  node->table()->addVariable(node->id(), std::move(varType), value, alloca);

  // Devolver el valor almacenado
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeVariableCall *node) const {
  std::cout << "---------\n" << *node->table() << std::flush;
  return node->table()->variableValue(node->id());
}

llvm::Value *CodeGeneration::visit(const NodeVariableReassignment *node) const {
  llvm::IRBuilder<> builder{entry_}; // Get the context of the current block

  llvm::AllocaInst *varAddress{node->table()->variableAdress(node->id())};
  llvm::Value *newValue{node->expression()->accept(this)};
  builder.CreateStore(newValue, varAddress);
  node->table()->setVariable(node->id(), newValue);

  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeIfStatement *node) const {
  llvm::IRBuilder<> builder{entry_};
  llvm::Value *condition = node->condition()->accept(this);
  if (!condition)
    return nullptr;
  // Convertir la condición a un valor booleano (si es necesario)
  condition = builder.CreateICmpNE(
      condition, llvm::ConstantInt::get(llvm::Type::getInt1Ty(*context_), 0),
      "ifcond");
  llvm::Function *parentFunction = builder.GetInsertBlock()->getParent();
  // Crear los bloques básicos para 'then', 'else' (si existe) y 'merge'
  llvm::BasicBlock *thenBlock =
      llvm::BasicBlock::Create(*context_, "then", parentFunction);
  llvm::BasicBlock *elseBlock =
      node->elseBody() ? llvm::BasicBlock::Create(*context_, "else") : nullptr;
  llvm::BasicBlock *mergeBlock = llvm::BasicBlock::Create(*context_, "ifcont");
  // Crear la rama condicional
  if (elseBlock) {
    builder.CreateCondBr(condition, thenBlock, elseBlock);
  } else {
    builder.CreateCondBr(condition, thenBlock, mergeBlock);
  }
  // Generar el código para el bloque 'then'
  builder.SetInsertPoint(thenBlock);
  llvm::Value *thenValue = node->body()->accept(this);
  if (!thenValue)
    return nullptr;
  builder.CreateBr(mergeBlock); // Saltar al bloque de 'merge' después de 'then'
  // Actualizar el bloque de 'then'
  thenBlock = builder.GetInsertBlock();
  // Generar el código para el bloque 'else' si existe
  llvm::Value *elseValue = nullptr;
  if (elseBlock) {
    parentFunction->insert(parentFunction->end(), elseBlock);
    builder.SetInsertPoint(elseBlock);
    elseValue = node->elseBody()->accept(this);
    if (!elseValue)
      return nullptr;
    builder.CreateBr(
        mergeBlock); // Saltar al bloque de 'merge' después de 'else'
    // Actualizar el bloque de 'else'
    elseBlock = builder.GetInsertBlock();
  }
  // Insertar el bloque de 'merge'
  parentFunction->insert(parentFunction->end(), mergeBlock);
  builder.SetInsertPoint(mergeBlock);
  // Si el 'if' produce un valor, debemos crear un PHI node para unir las dos
  // ramas
  if (thenValue || elseValue) {
    llvm::PHINode *phiNode =
        builder.CreatePHI(thenValue->getType(), 2, "iftmp");
    phiNode->addIncoming(thenValue, thenBlock);
    if (elseBlock) {
      phiNode->addIncoming(elseValue, elseBlock);
    } else {
      // Si no hay 'else', el 'then' es el único valor
      phiNode->addIncoming(llvm::UndefValue::get(thenValue->getType()),
                           thenBlock);
    }
    return phiNode; // Retornar el valor combinado del 'if'
  }
  // Si no produce un valor, retornar nullptr
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeStatementList *node) const {
  llvm::IRBuilder<> builder{entry_};
  llvm::Value *lastValue{nullptr};
  for (const auto &statement : *node) {
    llvm::Value *value{statement->accept(this)};
    if (statement->expression()->type() == NodeType::VAR_DECL ||
        statement->expression()->type() == NodeType::VAR_REG) {
      continue;
    }
    if (!value) {
      return nullptr;
    }
    lastValue = value;
  }
  // return lastValue;
  return builder.CreateRetVoid();
}

llvm::Value *CodeGeneration::visit(const Tree *node) const {
  // not using expression leads to infinite loop
  return node->root()->accept(this);
}

} // namespace nicole

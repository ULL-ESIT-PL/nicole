#include "../../inc/visitors/codeGeneration.h"

#include "../../inc/lexicalAnalysis/type.h"
#include "../../inc/parsingAnalysis/ast/calls/variableCall.h"
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

namespace nicole {

llvm::Value* CodeGeneration::visit(const NodeLiteralBool* node) const {
  return llvm::ConstantInt::get(llvm::Type::getInt1Ty(*context_),
                                node->value());
}

llvm::Value* CodeGeneration::visit(const NodeLiteralChar* node) const {
  return llvm::ConstantInt::get(llvm::Type::getInt8Ty(*context_),
                                node->value());
}

llvm::Value* CodeGeneration::visit(const NodeLiteralDouble* node) const {
  return llvm::ConstantFP::get(*context_, llvm::APFloat(node->value()));
}

llvm::Value* CodeGeneration::visit(const NodeLiteralInt* node) const {
  return llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context_),
                                node->value(), true);
}

llvm::Value* CodeGeneration::visit(const NodeLiteralString* node) const {
  llvm::IRBuilder<> builder{*context_};
  llvm::Value* str{builder.CreateGlobalString(llvm::StringRef{node->value()},
                                              "str", 0U, module_)};
  return builder.CreatePointerCast(str, builder.getInt8PtrTy());
}

llvm::Value* CodeGeneration::visit(const NodeBinaryOp* node) const {
  Node* left{node->left()};
  Node* right{node->right()};

  llvm::Value* leftEvaluated{left->accept(this)};
  llvm::Value* rightEvaluated{right->accept(this)};
  if (!leftEvaluated || !rightEvaluated) {
    return nullptr;
  }
  llvm::IRBuilder<> builder(*context_);
  // Convertir tipos si son diferentes (int a double)
  llvm::Type* leftType = leftEvaluated->getType();
  llvm::Type* rightType = rightEvaluated->getType();

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
    return nullptr;  // Error: tipos no compatibles
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
    default:
      return nullptr;
  }
}

llvm::Value* CodeGeneration::visit(const NodeStatement* node) const {
  // not using expression leads to infinite loop
  return node->expression()->accept(this);
}

llvm::Value* CodeGeneration::visit(const NodeStatementList* node) const {
  llvm::Value* lastValue{nullptr};
  for (const auto& statement : *node) {
    llvm::Value* value{statement->accept(this)};
    if (statement->expression()->type() == NodeType::VAR_DECL ||
        statement->expression()->type() == NodeType::VAR_REG) {
      std::cout << "hola\n" << std::flush;
      continue;
    }
    if (!value) {
      return nullptr;
    }
    lastValue = value;
  }
  
  return lastValue;
}

llvm::Value* CodeGeneration::visit(const NodeVariableDeclaration* node) const {
  llvm::IRBuilder<> builder{entry_};  // Obtener el contexto del módulo

  llvm::Value* value{node->expression()->accept(this)};
  llvm::Type* valueType{value->getType()};  // Tipo de la variable

  // Crear la instrucción 'alloca' para reservar espacio para la variable
  llvm::AllocaInst* alloca{
      builder.CreateAlloca(valueType, nullptr, node->id())};

  // Almacenar el valor en la variable y tambien en la tabla
  builder.CreateStore(value, alloca);
  std::unique_ptr<GenericType> varType(node->type());
  node->table()->addVariable(node->id(), std::move(varType), value, alloca);

  // Devolver el valor almacenado
  return nullptr;
}

llvm::Value* CodeGeneration::visit(const NodeVariableCall* node) const {
  return node->table()->variableValue(node->id());
}

llvm::Value* CodeGeneration::visit(const NodeVariableReassignment* node) const {
  llvm::IRBuilder<> builder{entry_};  // Get the context of the current block

  llvm::AllocaInst* varAddress{node->table()->variableAdress(node->id())};
  llvm::Value* newValue{node->expression()->accept(this)};
  builder.CreateStore(newValue, varAddress);
  node->table()->setVariable(node->id(), newValue);

  return nullptr;
}

llvm::Value* CodeGeneration::visit(const Tree* node) const {
  // not using expression leads to infinite loop
  return node->root()->accept(this);
}

}  // namespace nicole

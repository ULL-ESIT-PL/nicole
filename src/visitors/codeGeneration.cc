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
  // Here the attributes of node should be null except currentScope due being
  // owned by its attribute currentScope
  llvm::Value* value{node->table()->variableValue(node->id())->accept(this)};

  llvm::Type* valueType{value->getType()};  // Tipo de la variable

  // Crear la instrucción 'alloca' para reservar espacio para la variable
  llvm::AllocaInst* alloca{
      builder.CreateAlloca(valueType, nullptr, node->id())};

  // std::cout << node->table()->variableType(node->id())->name();

  // Almacenar el valor en la variable
  builder.CreateStore(value, alloca);

  // Devolver el valor almacenado
  return alloca;
}

llvm::Value* CodeGeneration::visit(const NodeVariableCall* node) const {
  llvm::IRBuilder<> builder{entry_};  // Obtener el contexto del bloque actual

  // Paso 1: Buscar la variable en la tabla de símbolos actual (currentScope_)
  llvm::Value* variable =
      node->table()->variableValue(node->id())->accept(this);

  // Verificar si la variable fue encontrada
  if (!variable) {
    std::cerr << "Error: Variable '" << node->id() << "' no encontrada."
              << std::endl;
    return nullptr;
  }

  // Paso 2: Cargar el valor almacenado en la variable
  llvm::Type* varType = variable->getType();
  llvm::Value* loadedValue = builder.CreateLoad(varType, variable, "loadtmp");

  // Paso 3: Devolver el valor cargado
  return loadedValue;
}

llvm::Value* CodeGeneration::visit(const NodeVariableReassignment* node) const {
  llvm::IRBuilder<> builder{entry_};  // Get the context of the current block

  // Retrieve the memory address (alloca) of the variable from the variable
  // table
  llvm::Value* varAddress{
      node->table()->variableValue(node->id())->accept(this)};

  std::unique_ptr<Node> expression{node->expression()};
  node->table()->setVariable(node->id(), std::move(expression));

  llvm::Value* newValue{
      node->table()->variableValue(node->id())->accept(this)};

  // Store the new value in the variable's existing memory location
  builder.CreateStore(newValue, varAddress);

  // Return the new value (not strictly necessary, but can be useful for further
  // operations)
  return nullptr;
}

llvm::Value* CodeGeneration::visit(const Tree* node) const {
  // not using expression leads to infinite loop
  return node->root()->accept(this);
}

}  // namespace nicole

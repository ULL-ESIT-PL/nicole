#include "../../inc/visitors/codeGeneration.h"

#include "../../inc/lexicalAnalysis/type.h"
#include "../../inc/parsingAnalysis/ast/calls/variableCall.h"
#include "../../inc/parsingAnalysis/ast/conditionals/NodeIfStatetement.h"
#include "../../inc/parsingAnalysis/ast/declaration/constDeclaration.h"
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
#include "llvm/IR/Instruction.h"
#include "llvm/Support/ErrorHandling.h"
#include <memory>

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
  llvm::Constant *strConst = llvm::ConstantDataArray::getString(
      *context_, node->value(), /*AddNull=*/true);

  llvm::Value *globalString{
      builder_.CreateGlobalString(llvm::StringRef{node->value()}, "str", 0U)};
  // Obtener el puntero al string global
  llvm::Value *globalStrPtr = builder_.CreatePointerCast(
      globalString, llvm::PointerType::getUnqual(strConst->getType()));

  return globalStrPtr;
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

  // Asegúrate de que ambos operandos tengan el mismo tipo ahora (ambos double o
  // ambos int)
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
      return builder_.CreateURem(leftEvaluated, rightEvaluated, "divtmp");
    }
  case TokenType::OR:
    if (leftEvaluated->getType()->isIntegerTy()) {
      return builder_.CreateOr(leftEvaluated, rightEvaluated, "divmp");
    }
  case TokenType::AND:
    if (leftEvaluated->getType()->isIntegerTy()) {
      return builder_.CreateAnd(leftEvaluated, rightEvaluated, "divmp");
    }
  case TokenType::EQUAL:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder_.CreateFCmpOEQ(leftEvaluated, rightEvaluated, "divmp");
    } else {
      return builder_.CreateICmpEQ(leftEvaluated, rightEvaluated, "divtmp");
    }
  case TokenType::NOTEQUAL:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder_.CreateFCmpONE(leftEvaluated, rightEvaluated, "divmp");
    } else {
      return builder_.CreateICmpNE(leftEvaluated, rightEvaluated, "divtmp");
    }
  case TokenType::OPERATOR_SMALLER:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder_.CreateFCmpOLT(leftEvaluated, rightEvaluated, "divmp");
    } else {
      return builder_.CreateICmpSLT(leftEvaluated, rightEvaluated, "divtmp");
    }
  case TokenType::SMALLEREQUAL:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder_.CreateFCmpOLE(leftEvaluated, rightEvaluated, "divmp");
    } else {
      return builder_.CreateICmpSLE(leftEvaluated, rightEvaluated, "divtmp");
    }
  case TokenType::OPERATOR_GREATER:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder_.CreateFCmpOGT(leftEvaluated, rightEvaluated, "divmp");
    } else {
      return builder_.CreateICmpSGT(leftEvaluated, rightEvaluated, "divtmp");
    }
  case TokenType::BIGGEREQUAL:
    if (leftEvaluated->getType()->isFloatingPointTy()) {
      return builder_.CreateFCmpOGE(leftEvaluated, rightEvaluated, "divmp");
    } else {
      return builder_.CreateICmpSGE(leftEvaluated, rightEvaluated, "divtmp");
    }
  default:
    return nullptr;
  }
}

llvm::Value *CodeGeneration::visit(const NodeStatement *node) const {
  auto val{node->expression()->accept(this)};
  return val;
}

llvm::Value *CodeGeneration::visit(const NodeVariableDeclaration *node) const {
  llvm::Value *value{node->expression()->accept(this)};
  llvm::Type *valueType{value->getType()}; // Tipo de la variable

  // Crear la instrucción 'alloca' para reservar espacio para la variable
  llvm::AllocaInst *alloca{
      builder_.CreateAlloca(valueType, nullptr, node->id())};

  // Almacenar el valor en la variable y tambien en la tabla
  builder_.CreateStore(value, alloca);
  const GenericType* varType{node->varType()};
  node->table()->addVariable(node->id(), varType, value, alloca);
  // Devolver el valor almacenado
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeConstDeclaration *node) const {
  llvm::Value *value{node->expression()->accept(this)};
  llvm::Type *valueType{value->getType()}; // Tipo de la variable

  // Crear la instrucción 'alloca' para reservar espacio para la variable
  llvm::AllocaInst *alloca{
      builder_.CreateAlloca(valueType, nullptr, node->id())};

  // Almacenar el valor en la variable y tambien en la tabla
  builder_.CreateStore(value, alloca);
  const GenericType* varType{node->varType()};
  node->table()->addVariable(node->id(), varType, value, alloca,
                             true);
  // Devolver el valor almacenado
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeVariableCall *node) const {
  std::cout << "---------\n" << *node->table() << std::flush;
  return node->table()->variableValue(node->id());
}

llvm::Value *CodeGeneration::visit(const NodeVariableReassignment *node) const {
  llvm::AllocaInst *varAddress{node->table()->variableAdress(node->id())};
  llvm::Value *newValue{node->expression()->accept(this)};
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
  builder_.CreateBr(MergeBB); // Unir con MergeBB

  if (ElseBB) {
    // Insertar el bloque 'else'
    TheFunction->insert(TheFunction->end(), ElseBB);
    builder_.SetInsertPoint(ElseBB);
    node->elseBody()->accept(this); // Ejecutar el cuerpo del 'else'
    builder_.CreateBr(MergeBB);     // Unir con MergeBB
  }

  // Insertar el bloque 'merge'
  TheFunction->insert(TheFunction->end(), MergeBB);
  builder_.SetInsertPoint(MergeBB);
  // No devolver valor ya que el 'if' solo controla el flujo
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeStatementList *node) const {
  llvm::Value *lastValue{nullptr};
  for (const auto &statement : *node) {
    llvm::Value *value{statement->accept(this)};
    if (statement->expression()->type() == NodeType::VAR_DECL ||
        statement->expression()->type() == NodeType::CONST_DECL ||
        statement->expression()->type() == NodeType::VAR_REG ||
        statement->expression()->type() == NodeType::IF) {
      // std::cout << "SKIPPED->>>"
      //          << nodeTypeToString(statement->expression()->type()) + "\n"
      //          << std::flush;
      continue;
    }
    if (!value) {
      return nullptr;
    }
    std::cout << "NOT SKIPPED->>>"
              << nodeTypeToString(statement->expression()->type()) + "\n"
              << std::flush;
    lastValue = value;
  }

  return lastValue;
}

llvm::Value *CodeGeneration::visit(const Tree *node) const {
  llvm::Value *val{node->root()->accept(this)};
  return builder_.CreateRetVoid();
}

llvm::Value *CodeGeneration::generate(const Tree *tr) const {
  std::cout << "Generate--> " << builder_.GetInsertBlock()->getName().str()
            << "\n";
  auto val{visit(tr)};
  std::cout << "Generate--> " << builder_.GetInsertBlock()->getName().str()
            << "\n";
  return val;
}
} // namespace nicole

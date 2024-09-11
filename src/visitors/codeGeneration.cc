#include "../../inc/visitors/codeGeneration.h"

#include "../../inc/parsingAnalysis/literals/nodeLiteralBool.h"
#include "../../inc/parsingAnalysis/literals/nodeLiteralChar.h"
#include "../../inc/parsingAnalysis/literals/nodeLiteralDouble.h"
#include "../../inc/parsingAnalysis/literals/nodeLiteralInt.h"
#include "../../inc/parsingAnalysis/literals/nodeLiteralString.h"
#include "../../inc/parsingAnalysis/operations/nodeBinary.h"
#include "../../inc/parsingAnalysis/statements/statement.h"
#include <iostream>

namespace nicole {
llvm::Value* CodeGeneration::visit(const NodeLiteralBool* node) const {
  return llvm::ConstantInt::get(llvm::Type::getInt1Ty(*context_), node->value());
}

llvm::Value* CodeGeneration::visit(const NodeLiteralChar* node) const {
  return llvm::ConstantInt::get(llvm::Type::getInt8Ty(*context_), node->value());
}

llvm::Value* CodeGeneration::visit(const NodeLiteralDouble* node) const {
  return llvm::ConstantFP::get(*context_, llvm::APFloat(node->value()));
}

llvm::Value* CodeGeneration::visit(const NodeLiteralInt* node) const {
  std::cout << "val: " << node->value() << "\n" << std::flush;
  return llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context_),
                                node->value(), true);
}

llvm::Value* CodeGeneration::visit(const NodeLiteralString* node) const {
  llvm::IRBuilder<> builder{*context_};
  llvm::Value* str{builder.CreateGlobalString(llvm::StringRef{node->value()}, "str", 0U, module_)};
  return builder.CreatePointerCast(str, builder.getInt8PtrTy());
}

llvm::Value* CodeGeneration::visit(const NodeBinary* node) const {
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
    case Operator::ADD:
      if (leftEvaluated->getType()->isFloatingPointTy()) {
        return builder.CreateFAdd(leftEvaluated, rightEvaluated, "addtmp");
      } else {
        return builder.CreateAdd(leftEvaluated, rightEvaluated, "addtmp");
      }
    case Operator::SUB:
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
}  // namespace nicole

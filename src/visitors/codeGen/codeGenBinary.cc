#include "../../../inc/visitors/codeGeneration.h"

#include "../../../inc/lexicalAnalysis/type.h"
#include "../../../inc/parsingAnalysis/ast/operations/nodeBinaryOp.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/ErrorHandling.h"

namespace nicole {

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

} // namespace nicole
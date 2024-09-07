#include "../../inc/parser/nodeBinary.h"

namespace nicole {
llvm::Value* NodeBinary::codeGeneration() const {
  llvm::Value* leftEvaluated{left_->codeGeneration()};
  llvm::Value* rightEvaluated{right_->codeGeneration()};
  if (!leftEvaluated || !rightEvaluated) {
    return nullptr;
  }
  llvm::IRBuilder<> builder(*context_);
  // Convertir tipos si son diferentes (int a double)
  llvm::Type* leftType = leftEvaluated->getType();
  llvm::Type* rightType = rightEvaluated->getType();

  // Si left es int y right es double, convierte left a double
  if (leftType->isIntegerTy() && rightType->isFloatingPointTy()) {
    leftEvaluated = builder.CreateSIToFP(leftEvaluated, rightType, "intToDouble");
  }
  // Si left es double y right es int, convierte right a double
  else if (leftType->isFloatingPointTy() && rightType->isIntegerTy()) {
    rightEvaluated = builder.CreateSIToFP(rightEvaluated, leftType, "intToDouble");
  }

  // Asegúrate de que ambos operandos tengan el mismo tipo ahora (ambos double o ambos int)
  if (leftEvaluated->getType() != rightEvaluated->getType()) {
    return nullptr; // Error: tipos no compatibles
  }

  // Realiza la operación basada en el tipo del operador
  switch (operator_) {
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
}  // namespace nicole

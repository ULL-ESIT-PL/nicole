#include "../../inc/parser/nodeBinary.h"

namespace nicole {
llvm::Value* NodeBinary::codeGeneration() const {
  llvm::Value* leftEvaluated{left_->codeGeneration()};
  llvm::Value* rightEvaluated{right_->codeGeneration()};
  if (!leftEvaluated || !rightEvaluated) {
    return nullptr;
  }
  llvm::IRBuilder<> builder(*context_);
  switch (operator_) {
    case Operator::ADD:
      return builder.CreateFAdd(leftEvaluated, rightEvaluated, "addtmp");
    case Operator::SUB:
      return builder.CreateFAdd(leftEvaluated, rightEvaluated, "subtmp");
    default:
      return nullptr;
  }
}
}  // namespace nicole

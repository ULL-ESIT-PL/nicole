#include "../../inc/parser/nodeLiteral.h"

namespace nicole {
llvm::Value* NodeLiteral::codeGeneration() const {
  return llvm::ConstantFP::get(*context_, llvm::APFloat(value_));
}
}  // namespace nicole

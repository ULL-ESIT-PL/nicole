#include "../../inc/parser/nodeLiteralBool.h"

namespace nicole {
llvm::Value* NodeLiteralBool::codeGeneration() const {
  return llvm::ConstantInt::get(llvm::Type::getInt1Ty(*context_), value_);
}
}  // namespace nicole

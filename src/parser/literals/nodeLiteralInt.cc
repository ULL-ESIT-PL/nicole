#include "../../../inc/parser/literals/nodeLiteralInt.h"

namespace nicole {
llvm::Value* NodeLiteralInt::codeGeneration() const {
  return llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context_), value_, true);
}
}  // namespace nicole

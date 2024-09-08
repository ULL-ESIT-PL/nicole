#include "../../../inc/parsingAnalysis/literals/nodeLiteralChar.h"

namespace nicole {
llvm::Value* NodeLiteralChar::codeGeneration() const {
  return llvm::ConstantInt::get(llvm::Type::getInt8Ty(*context_), value_);
}
}  // namespace nicole

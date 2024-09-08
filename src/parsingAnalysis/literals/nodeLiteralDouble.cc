#include "../../../inc/parsingAnalysis/literals/nodeLiteralDouble.h"

namespace nicole {
llvm::Value* NodeLiteralDouble::codeGeneration() const {
  return llvm::ConstantFP::get(*context_, llvm::APFloat(value_));
}
}  // namespace nicole

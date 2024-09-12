#include "../../../inc/parsingAnalysis/parsingAlgorithms/parser.h"

namespace nicole {
void Parser::eat() const {
  if (!(currentToke_ < tokens_.size()))
    llvm::report_fatal_error("Error: invalid access to tokens while eating");
  ++currentToke_;
}

Token Parser::getCurrentToke() const {
  if (currentToke_ < tokens_.size()) return tokens_[currentToke_];
  llvm::report_fatal_error("Error: invalid access to tokens");
}
}  // namespace nicole

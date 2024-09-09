#include "../../../inc/parsingAnalysis/parsingAlgorithms/parser.h"

namespace nicole {
void Parser::eat() const {
  if (currentToke_ < tokens_.size()) ++currentToke_;
}

Token Parser::getCurrentToke() const {
  assert(currentToke_ < tokens_.size());
  return tokens_[currentToke_];
}
}  // namespace nicole

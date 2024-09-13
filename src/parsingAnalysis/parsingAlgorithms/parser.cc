#include "../../../inc/parsingAnalysis/parsingAlgorithms/parser.h"

namespace nicole {
void Parser::eat() const {
  if (!(currentToken_ < tokens_.size()))
    llvm::report_fatal_error("Error: invalid access to tokens while eating");
  ++currentToken_;
}

Token Parser::getCurrentToken() const {
  if (currentToken_ < tokens_.size()) return tokens_[currentToken_];
  llvm::report_fatal_error("Error: invalid access to tokens");
}

bool Parser::isTokenType(const TokenType& type) const {
  if (currentToken_ < tokens_.size())
    return tokens_[currentToken_].type() == type;
  llvm::report_fatal_error("Error: invalid access to tokens");
}
}  // namespace nicole

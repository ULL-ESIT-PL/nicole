#include "../../inc/lexicalAnalysis/tokeStream.h"
#include <cstddef>
#include <limits>

namespace nicole {

void TokenStream::eat() const {
  if (currentPos_ <= tokens_.size()) {
    ++currentPos_;
    return;
  }
  llvm::report_fatal_error("Error: invalid access to tokens while eating");
}

bool TokenStream::isEnd() const { return currentPos_ == tokens_.size(); }

Token TokenStream::current() const {
  if (currentPos_ < tokens_.size())
    return tokens_[currentPos_];
  llvm::report_fatal_error("Error: invalid access to tokens");
}

Token TokenStream::lookAhead(const size_t pos) const {
  if (currentPos_ + pos < tokens_.size())
    return tokens_[currentPos_ + pos];
  llvm::report_fatal_error("Error: invalid access to tokens");
}

bool TokenStream::isCurrentTokenType(const TokenType type) const {
  if (currentPos_ < tokens_.size())
    return tokens_[currentPos_].type() == type;
  llvm::report_fatal_error("Error: invalid access to tokens");
}

bool TokenStream::isTokenAheadBeforeSemicolon(const TokenType type) const {
  bool foundToken{false};
  for (size_t i{currentPos_}; i < tokens_.size(); ++i) {
    auto tk{tokens_[i]};
    if (tk.type() == type) {
      foundToken = true;
      break;
    }
    if (tk.type() == TokenType::SEMICOLON) {
      break;
    }
  }
  return foundToken;
}

void TokenStream::insertAfter(const TokenStream &tkStream, size_t pos) const {
  if (pos == std::numeric_limits<int>::infinity()) {
    pos = currentPos_;
  }
  tokens_.insert(tokens_.begin() + pos, tkStream.begin(),
                 tkStream.end());
}

} // namespace nicole
#include "../../inc/lexicalAnalysis/tokenStream.h"
#include <cstddef>
#include <expected>
#include <limits>

namespace nicole {

std::expected<void, Error> TokenStream::eat() const {
  if (currentPos_ <= tokens_.size()) {
    ++currentPos_;
    return std::expected<void, Error>{};
  }
  return std::unexpected{
      Error{ERROR_TYPE::EAT, "invalid access to tokens while eating"}};
  // llvm::report_fatal_error("Error: invalid access to tokens while eating");
}

bool TokenStream::isEnd() const { return currentPos_ == tokens_.size(); }

std::expected<Token, Error> TokenStream::current() const {
  if (currentPos_ < tokens_.size())
    return tokens_[currentPos_];
  // llvm::report_fatal_error("Error: invalid access to tokens");
  return std::unexpected{
      Error{ERROR_TYPE::CURRENT, "invalid access to tokens"}};
}

std::expected<Token, Error> TokenStream::lookAhead(const size_t pos) const {
  if (currentPos_ + pos < tokens_.size())
    return tokens_[currentPos_ + pos];
  // llvm::report_fatal_error("Error: invalid access to tokens");
  return std::unexpected{
      Error{ERROR_TYPE::LOOK_AHEAD, "invalid access to tokens"}};
}

std::expected<bool, Error>
TokenStream::isCurrentTokenType(const TokenType type) const {
  if (currentPos_ < tokens_.size())
    return tokens_[currentPos_].type() == type;
  // llvm::report_fatal_error("Error: invalid access to tokens");
  return std::unexpected{
      Error{ERROR_TYPE::IS_CURRENT_TOKEN_TYPE, "invalid access to tokens"}};
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

std::expected<void, Error> TokenStream::insertAfter(const TokenStream &tkStream,
                                                    const size_t pos) const {
  if (pos == std::numeric_limits<int>::infinity()) {
    return std::unexpected{Error{ERROR_TYPE::INSERT_AFTER,
                                 "cannot insert after the given position"}};
  }
  tokens_.insert(tokens_.begin() + static_cast<long>(pos), tkStream.begin(), tkStream.end());
  return std::expected<void, Error>{};
}

} // namespace nicole
#include "../../inc/lexicalAnalysis/tokenStream.h"
#include <cstddef>

namespace nicole {

const std::expected<std::monostate, Error> TokenStream::eat() noexcept {
  if (currentPos_ < tokens_.size()) {
    ++currentPos_;
    return std::expected<std::monostate, Error>{std::monostate{}};
  }
  return std::unexpected{
      Error{ERROR_TYPE::EAT, "invalid access to tokens while eating"}};
}

bool TokenStream::isEnd() const noexcept {
  return currentPos_ == tokens_.size();
}

const std::expected<Token, Error> TokenStream::current() const noexcept {
  if (currentPos_ < tokens_.size())
    return tokens_[currentPos_];
  return std::unexpected{
      Error{ERROR_TYPE::CURRENT, "invalid access to tokens"}};
}

const std::expected<Token, Error>
TokenStream::lookAhead(const size_t pos) const noexcept {
  if (currentPos_ + pos < tokens_.size())
    return tokens_[currentPos_ + pos];
  return std::unexpected{
      Error{ERROR_TYPE::LOOK_AHEAD, "invalid access to tokens"}};
}

const std::expected<Token, Error> TokenStream::lastRead() const noexcept {
  if (currentPos_  - 1 < tokens_.size())
    return tokens_[currentPos_ - 1];
  return std::unexpected{
      Error{ERROR_TYPE::LAST_READ, "invalid access to tokens"}};
}

const std::expected<bool, Error>
TokenStream::isCurrentTokenType(const TokenType type) const noexcept {
  if (currentPos_ < tokens_.size())
    return tokens_[currentPos_].type() == type;
  return std::unexpected{
      Error{ERROR_TYPE::IS_CURRENT_TOKEN_TYPE, "invalid access to tokens"}};
}

bool TokenStream::isTokenAheadBeforeSemicolon(
    const TokenType type) const noexcept {
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

const std::expected<std::monostate, Error>
TokenStream::insertAfter(const TokenStream &tkStream,
                         const size_t pos) noexcept {
  if (pos == std::numeric_limits<int>::infinity()) {
    return std::unexpected{Error{ERROR_TYPE::INSERT_AFTER,
                                 "cannot insert after the given position"}};
  }
  tokens_.insert(tokens_.begin() + static_cast<long>(pos), tkStream.begin(),
                 tkStream.end());
  return std::expected<std::monostate, Error>{std::monostate{}};
}

void TokenStream::shiftToSemicolon() noexcept {
  while (currentPos_ < size()) {
    if (tokens_[currentPos_].type() == TokenType::SEMICOLON) {
      break;
    }
    ++currentPos_;
  }
}

} // namespace nicole
#ifndef TOKEN_STREAM_H
#define TOKEN_STREAM_H

#include "../errors.h"
#include "token.h"
#include "type.h"
#include <cstddef>
#include <expected>
#include <limits>
#include <variant>
#include <vector>

namespace nicole {

// represents the tokens returned by the lexer
class TokenStream final {
private:
  std::vector<Token> tokens_;
  std::size_t currentPos_{0};

public:
  explicit TokenStream(const std::vector<Token> &tokens) noexcept
      : tokens_{tokens} {}

  [[nodiscard]] size_t size() const noexcept { return tokens_.size(); };

  [[nodiscard]] const std::expected<std::monostate, Error> eat() noexcept;

  [[nodiscard]] size_t currentPos() const noexcept { return currentPos_; }

  [[nodiscard]] bool isEnd() const noexcept;

  [[nodiscard]] const std::expected<Token, Error> current() const noexcept;

  [[nodiscard]] const std::expected<Token, Error>
  lookAhead(const size_t pos) const noexcept;

  [[nodiscard]] const std::expected<Token, Error> lastRead() const noexcept;

  [[nodiscard]] const std::expected<bool, Error>
  isCurrentTokenType(const TokenType type) const noexcept;

  [[nodiscard]] bool
  isTokenAheadBeforeSemicolon(const TokenType type) const noexcept;

  // needed for whenever an import is found
  [[nodiscard]] const std::expected<std::monostate, Error>
  insertAfter(const TokenStream &tkStream,
              const size_t pos = std::numeric_limits<int>::infinity()) noexcept;

  void shiftToSemicolon() noexcept;

  [[nodiscard]] auto begin() const noexcept { return tokens_.begin(); }

  [[nodiscard]] auto end() const noexcept { return tokens_.end(); }

  [[nodiscard]] bool hasMatchingPairBefore(TokenType open,
                                           TokenType until) const noexcept ;
};

} // namespace nicole

#endif
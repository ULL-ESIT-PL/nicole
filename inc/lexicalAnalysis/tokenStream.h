#ifndef TOKEN_STREAM_H
#define TOKEN_STREAM_H

#include "../errors.h"
#include "token.h"
#include "type.h"
#include <cstddef>
#include <expected>
#include <limits>
#include <vector>

namespace nicole {

// represents the tokens returned by the lexer
class TokenStream final {
private:
  mutable std::vector<Token> tokens_{};
  mutable std::size_t currentPos_{0};

public:
  explicit TokenStream(const std::vector<Token> &tokens) noexcept
      : tokens_{tokens} {}

  [[nodiscard]] size_t size() const noexcept { return tokens_.size(); };

  [[nodiscard]] std::expected<void, Error> eat() const noexcept;

  [[nodiscard]] std::expected<size_t, Error> currentPos() const noexcept {
    return currentPos_;
  }

  [[nodiscard]] bool isEnd() const noexcept;

  [[nodiscard]] std::expected<Token, Error> current() const noexcept;

  [[nodiscard]] std::expected<Token, Error>
  lookAhead(const size_t pos) const noexcept;

  [[nodiscard]] std::expected<bool, Error>
  isCurrentTokenType(const TokenType type) const noexcept;

  [[nodiscard]] bool
  isTokenAheadBeforeSemicolon(const TokenType type) const noexcept;

  // needed for whenever an import is found
  [[nodiscard]] std::expected<void, Error> insertAfter(
      const TokenStream &tkStream,
      const size_t pos = std::numeric_limits<int>::infinity()) const noexcept;

  [[nodiscard]] auto begin() const noexcept { return tokens_.begin(); }

  [[nodiscard]] auto end() const noexcept { return tokens_.end(); }
};

} // namespace nicole

#endif
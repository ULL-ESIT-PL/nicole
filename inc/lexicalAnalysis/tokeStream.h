#ifndef TOKE_STREAM_H
#define TOKE_STREAM_H

#include "../erros.h"
#include "token.h"
#include "type.h"
#include <cstddef>
#include <expected>
#include <limits>
#include <llvm/Support/ErrorHandling.h>
#include <vector>

namespace nicole {

// represents the tokens returned by the lexer
class TokenStream {
private:
  mutable std::vector<Token> tokens_{};
  mutable std::size_t currentPos_{0};

public:
  explicit TokenStream(const std::vector<Token> &tokens) noexcept
      : tokens_{tokens} {}

  [[nodiscard]] size_t size() const { return tokens_.size(); };

  [[nodiscard]] std::expected<void, Error> eat() const;

  [[nodiscard]] std::expected<size_t, Error> currentPos() const { return currentPos_; }

  [[nodiscard]] bool isEnd() const;

  [[nodiscard]] std::expected<Token, Error> current() const;

  [[nodiscard]] std::expected<Token, Error> lookAhead(const size_t pos) const;

  [[nodiscard]] std::expected<bool, Error> isCurrentTokenType(const TokenType type) const;

  [[nodiscard]] bool isTokenAheadBeforeSemicolon(const TokenType type) const;

  // needed for whenever an import is found
  [[nodiscard]] std::expected<void, Error> insertAfter(const TokenStream &tkStream,
                   const size_t pos = std::numeric_limits<int>::infinity()) const;

  [[nodiscard]] auto begin() const { return tokens_.begin(); }

  [[nodiscard]] auto end() const { return tokens_.end(); }
};

} // namespace nicole

#endif
#ifndef TOKE_STREAM_H
#define TOKE_STREAM_H

#include "token.h"
#include "type.h"
#include <cstddef>
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
  TokenStream(const std::vector<Token> &tokens) : tokens_{tokens} {}

  size_t size() const { return tokens_.size(); };

  void eat() const;

  size_t currentPos() const { return currentPos_; }

  bool isEnd() const;

  Token current() const;

  Token lookAhead(const size_t pos) const;

  bool isCurrentTokenType(const TokenType type) const;

  // needed for whenever an import is found
  void insertAfter(const TokenStream &tkStream,
                   size_t pos = std::numeric_limits<int>::infinity()) const;

  auto begin() const { return tokens_.begin(); }

  auto end() const { return tokens_.end(); }
};

} // namespace nicole

#endif
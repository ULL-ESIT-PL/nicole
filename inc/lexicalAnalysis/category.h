#ifndef CATEGORY_H
#define CATEGORY_H

#include <iostream>
#include <string>
#include <vector>

#include "regexWrapper.h"
#include "token.h"

namespace nicole {

// Represents de category of a token
class Category final {
private:
  TokenType type_{};
  RegexWrapper pattern_;
  bool skip_{false};

public:
  explicit Category(const TokenType &type, const std::string &matcher,
                    const bool skip) noexcept
      : type_{type}, pattern_{matcher}, skip_{skip} {};

  [[nodiscard]] TokenType type() const { return type_; }

  [[nodiscard]] RegexWrapper pattern() const { return pattern_; }

  [[nodiscard]] std::string rawPattern() const { return pattern_.str(); }

  // to know if it must be skipped like comments
  [[nodiscard]] bool skip() const { return skip_; }

  [[nodiscard]] bool matchToken(const Token &token) const {
    return regex_match(token.raw(), pattern_.pattern());
  };
};

} // namespace nicole

#endif
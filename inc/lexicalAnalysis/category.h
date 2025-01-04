#ifndef CATEGORY_H
#define CATEGORY_H

#include <string>

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

  [[nodiscard]] TokenType type() const noexcept { return type_; }

  [[nodiscard]] const RegexWrapper &pattern() const noexcept {
    return pattern_;
  }

  [[nodiscard]] const std::string rawPattern() const noexcept {
    return pattern_.str();
  }

  // to know if it must be skipped like comments
  [[nodiscard]] bool skip() const noexcept { return skip_; }

  [[nodiscard]] bool matchToken(const Token &token) const noexcept {
    return pattern_.match(token.raw());
  };
};

} // namespace nicole

#endif
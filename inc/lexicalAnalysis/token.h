#ifndef TOKEN_H
#define TOKEN_H

#include "location.h"
#include "type.h"
#include <string>

namespace nicole {

// Used to simplify the parsing analysis
class Token final {
private:
  TokenType type_{};
  std::string raw_{""};
  Location loc_{"", 0, 0};

public:
  explicit Token(const TokenType &type, const std::string &raw,
                 const Location &loc) noexcept
      : type_{type}, raw_{raw}, loc_{loc} {};

  [[nodiscard]] TokenType type() const noexcept { return type_; };

  [[nodiscard]] std::string raw() const noexcept { return raw_; }

  [[nodiscard]] Location location() const noexcept { return loc_; }

  [[nodiscard]] std::string locInfo() const noexcept {
    return loc_.file().string() + " " + std::to_string(loc_.row()) + ':' +
           std::to_string(loc_.col());
  }
};

} // namespace nicole

#endif
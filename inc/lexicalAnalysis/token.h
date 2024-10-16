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
  Token(const TokenType &type, const std::string &raw, const Location &loc)
      : type_{type}, raw_{raw}, loc_{loc} {};

  TokenType type() const { return type_; };

  std::string raw() const { return raw_; }

  Location location() const { return loc_; }

  std::string locInfo() const {
    return loc_.file().string() + " " + std::to_string(loc_.row()) + ':' +
           std::to_string(loc_.col());
  }
};

} // namespace nicole

#endif
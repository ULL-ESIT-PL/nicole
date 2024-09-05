#ifndef TOKEN_H
#define TOKEN_H

#include "tokenType.h"
using namespace std;

namespace nicole {

class Token {
 private:
  TokenType type_{TokenType::UNDEFINED};
  std::string raw_{};

 public:
  Token(const TokenType& type, const string& raw)
      : type_{type}, raw_{raw} {};
  TokenType type() const { return type_; }
  std::string raw() const { return raw_; }
};

}  // namespace nicole

#endif

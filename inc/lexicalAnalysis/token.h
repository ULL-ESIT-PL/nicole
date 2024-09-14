#ifndef TOKEN_H
#define TOKEN_H

#include "type.h"

namespace nicole {

class Token final {
 private:
  TokenType type_{};
  std::string raw_{""};
  int row_{-1};
  int col_{-1};

 public:
  Token(const TokenType& type, const std::string& raw, int row, int col)
      : type_{type}, raw_{raw}, row_{row}, col_{col} {};

  TokenType type() const { return type_; };

  std::string raw() const { return raw_; }

  int row() const { return row_; }

  int col() const { return col_; }
};

}  // namespace nicole

#endif
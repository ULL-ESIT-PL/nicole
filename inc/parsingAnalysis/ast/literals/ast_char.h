#ifndef AST_CHAR_H
#define AST_CHAR_H

#include "../ast.h"
#include <string>

namespace nicole {

class AST_CHAR final : public AST {
private:
  std::string value_;

public:
  explicit AST_CHAR(const std::string &value) noexcept
      : AST{AST_TYPE::CHAR}, value_{value} {}

  ~AST_CHAR() noexcept = default;

  [[nodiscard]] const std::string &value() const noexcept { return value_; }
};

} // namespace nicole

#endif // AST_H

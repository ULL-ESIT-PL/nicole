#ifndef AST_INT_H
#define AST_INT_H

#include "../ast.h"

namespace nicole {

class AST_INT final : public AST {
private:
  int value_;

public:
  explicit AST_INT(const int value,
                   const SourceLocation &sourceLocation) noexcept
      : AST{AST_TYPE::INT, sourceLocation}, value_{value} {}

  ~AST_INT() noexcept = default;

  [[nodiscard]] int value() const noexcept { return value_; }
};

} // namespace nicole

#endif // AST_H

#ifndef AST_THIS_H
#define AST_THIS_H

#include "../ast.h"

namespace nicole {

class AST_THIS final : public AST {
public:
  explicit AST_THIS() noexcept : AST{AST_TYPE::THIS} {}

  ~AST_THIS() noexcept = default;

  [[nodiscard]] std::expected<std::string, Error>
  accept(const PrintTree &visitor) const noexcept override {
    return visitor.visit(this);
  }

  [[nodiscard]] std::expected<bool, Error>
  accept(const ValidateTree &visitor) const noexcept override {
    return visitor.visit(this);
  }
};

} // namespace nicole

#endif // AST_H

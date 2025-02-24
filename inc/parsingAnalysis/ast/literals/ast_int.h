#ifndef AST_INT_H
#define AST_INT_H

#include "../ast.h"

namespace nicole {

class AST_INT final : public AST {
private:
  int value_;

public:
  explicit AST_INT(const int value) noexcept
      : AST{AST_TYPE::INT}, value_{value} {}

  ~AST_INT() noexcept = default;

  [[nodiscard]] int value() const noexcept { return value_; }

  [[nodiscard]] std::expected<std::string, Error>
  accept(const PrintTree &visitor) const noexcept override {
    return visitor.visit(this);
  }

  [[nodiscard]] std::expected<bool, Error>
  accept(const ValidateTree &visitor) const noexcept override {
    return visitor.visit(this);
  }

  [[nodiscard]] std::expected<std::monostate, Error>
  accept(const FillScopes &visitor) const noexcept override {
    return visitor.visit(this);
  }
};

} // namespace nicole

#endif // AST_H

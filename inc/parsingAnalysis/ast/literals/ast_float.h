#ifndef AST_FLOAT_H
#define AST_FLOAT_H

#include "../ast.h"

namespace nicole {

class AST_FLOAT final : public AST {
private:
  float value_;

public:
  explicit AST_FLOAT(const float value) noexcept
      : AST{AST_TYPE::FLOAT}, value_{value} {}

  ~AST_FLOAT() noexcept = default;

  [[nodiscard]] float value() const noexcept { return value_; }

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

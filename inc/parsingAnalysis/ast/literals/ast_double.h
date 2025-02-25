#ifndef AST_DOUBLE_H
#define AST_DOUBLE_H

#include "../ast.h"

namespace nicole {

class AST_DOUBLE final : public AST {
private:
  double value_;

public:
  explicit AST_DOUBLE(const double value) noexcept
      : AST{AST_TYPE::DOUBLE}, value_{value} {}

  ~AST_DOUBLE() noexcept = default;

  [[nodiscard]] double value() const noexcept { return value_; }

  [[nodiscard]] std::expected<std::string, Error>
  accept(const PrintTree &visitor) const noexcept override {
    return visitor.visit(this);
  }

  [[nodiscard]] std::expected<bool, Error>
  accept(const ValidateTree &visitor) const noexcept override {
    return visitor.visit(this);
  }

  [[nodiscard]] std::expected<std::monostate, Error>
  accept(const FillSemanticInfo &visitor) const noexcept override {
    return visitor.visit(this);
  }
};

} // namespace nicole

#endif // AST_H

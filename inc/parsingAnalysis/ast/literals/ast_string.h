#ifndef AST_STRING_H
#define AST_STRING_H

#include "../ast.h"

namespace nicole {

class AST_STRING final : public AST {
private:
  std::string value_;

public:
  explicit AST_STRING(const std::string value) noexcept
      : AST{AST_TYPE::STRING}, value_{value} {}

  ~AST_STRING() noexcept = default;

  [[nodiscard]] const std::string &value() const noexcept { return value_; }

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

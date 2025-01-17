#ifndef AST_BOOL_H
#define AST_BOOL_H

#include "../ast.h"

namespace nicole {

class AST_BOOL final : public AST {
private:
  bool value_;

public:
  explicit AST_BOOL(const bool value) noexcept
      : AST{AST_TYPE::BOOL}, value_{value} {}

  ~AST_BOOL() noexcept = default;

  [[nodiscard]] bool value() const noexcept { return value_; }

  [[nodiscard]] std::expected<std::string, Error>
  accept(const PrintTree &visitor) const noexcept override {
    return visitor.visit(this);
  }
};

} // namespace nicole

#endif // AST_H

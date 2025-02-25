#ifndef AST_NULL_H
#define AST_NULL_H

#include "../ast.h"

namespace nicole {

class AST_NULL final : public AST {
public:
  explicit AST_NULL() noexcept : AST{AST_TYPE::NULLPTR} {}

  ~AST_NULL() noexcept = default;

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

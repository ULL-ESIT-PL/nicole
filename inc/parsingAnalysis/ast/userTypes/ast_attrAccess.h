#ifndef AST_ATTR_ACCESS_H
#define AST_ATTR_ACCESS_H

#include "../ast.h"

namespace nicole {

class AST_ATTR_ACCESS final : public AST {
private:
  std::string id_;

public:
  explicit AST_ATTR_ACCESS(const std::string id) noexcept
      : AST{AST_TYPE::ATTR_ACCESS}, id_{id} {}

  ~AST_ATTR_ACCESS() noexcept = default;

  [[nodiscard]] const std::string &id() const noexcept { return id_; }

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

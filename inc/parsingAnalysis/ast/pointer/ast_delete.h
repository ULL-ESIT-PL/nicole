#ifndef AST_DELETE_H
#define AST_DELETE_H

#include "../ast.h"
#include <memory>

namespace nicole {

class AST_DELETE : public AST {
private:
  std::shared_ptr<AST> value_;

public:
  explicit AST_DELETE(const std::shared_ptr<AST> &value) noexcept
      : AST(AST_TYPE::DELETE), value_{value} {}

  [[nodiscard]] const std::shared_ptr<AST> &value() const noexcept {
    return value_;
  }

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

#endif
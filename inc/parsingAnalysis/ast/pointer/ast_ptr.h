#ifndef AST_PTR_H
#define AST_PTR_H

#include "../ast.h"
#include <memory>

namespace nicole {

class AST_PTR : public AST {
private:
  std::string id_;
  std::string type_;
  std::shared_ptr<AST> value_;

public:
  explicit AST_PTR(const std::string &id, const std::string &type,
                   const std::shared_ptr<AST> &value) noexcept
      : AST(AST_TYPE::PTR), id_{id}, type_{type}, value_{value} {}

  [[nodiscard]] const std::string &id() const noexcept { return id_; }

  [[nodiscard]] const std::string &valueType() const noexcept { return type_; }

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
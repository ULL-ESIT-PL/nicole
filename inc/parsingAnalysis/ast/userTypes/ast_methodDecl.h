#ifndef AST_METHOD_DECL_H
#define AST_METHOD_DECL_H

#include "../../types/userTypes/genericParameter.h"
#include "../functions/ast_parametrizedSubritineDecl.h"
#include <memory>

namespace nicole {

class AST_METHOD_DECL final : public AST_PARAMETRIZED_SUBRUTINE_DECL {
private:
  std::vector<GenericParameter> generics_;
  bool isVirtual_;

public:
  explicit AST_METHOD_DECL(const std::string &id,
                           const std::vector<GenericParameter> &generics,
                           const Parameters &params,
                           const std::shared_ptr<Type> &returnType,
                           const std::shared_ptr<AST_BODY> &body,
                           const bool isVirtual) noexcept
      : AST_PARAMETRIZED_SUBRUTINE_DECL(AST_TYPE::METHOD_DECL, id, params,
                                        returnType, body),
        generics_{generics}, isVirtual_{isVirtual} {}

  [[nodiscard]] const std::vector<GenericParameter> &generics() const noexcept {
    return generics_;
  }

  [[nodiscard]] bool isVirtual() const noexcept { return isVirtual_; }

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
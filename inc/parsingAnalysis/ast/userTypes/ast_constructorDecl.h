#ifndef AST_CONSTRUCTOR_DECL_H
#define AST_CONSTRUCTOR_DECL_H

#include "../../../tables/typeTable/types/userTypes/genericParameter.h"
#include "../functions/ast_parametrizedSubritineDecl.h"
#include "ast_super.h"
#include <memory>

namespace nicole {

class AST_CONSTRUCTOR_DECL final : public AST_PARAMETRIZED_SUBRUTINE_DECL {
private:
  std::vector<GenericParameter> generics_;
  std::shared_ptr<AST_SUPER> super_;

public:
  explicit AST_CONSTRUCTOR_DECL(
      const std::string &id, const std::vector<GenericParameter> &generics,
      const Parameters &params,
      const std::shared_ptr<AST_SUPER> super,
      const std::shared_ptr<Type> &returnType,
      const std::shared_ptr<AST_BODY> &body) noexcept
      : AST_PARAMETRIZED_SUBRUTINE_DECL(AST_TYPE::CONSTRUCTOR_DECL, id, params,
                                        returnType, body),
        generics_{generics}, super_{super} {}

  [[nodiscard]] const std::vector<GenericParameter> &generics() const noexcept {
    return generics_;
  }

  [[nodiscard]] const std::shared_ptr<AST_SUPER> &super() const noexcept {
    return super_;
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
  accept(const FillSemanticInfo &visitor) const noexcept override {
    return visitor.visit(this);
  }
};

} // namespace nicole

#endif
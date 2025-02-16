#ifndef AST_CONSTRUCTOR_DECL_H
#define AST_CONSTRUCTOR_DECL_H

#include "../functions/ast_parametrizedSubritineDecl.h"

namespace nicole {

class AST_CONSTRUCTOR_DECL final : public AST_PARAMETRIZED_SUBRUTINE_DECL {
public:
  explicit AST_CONSTRUCTOR_DECL(const std::string &id_returnType,
                                const Parameters &params,
                                const std::shared_ptr<AST_BODY> &body) noexcept
      : AST_PARAMETRIZED_SUBRUTINE_DECL(AST_TYPE::CONSTRUCTOR_DECL, id_returnType, params,
                           id_returnType, body) {}

  [[nodiscard]] std::expected<std::string, Error>
  accept(const PrintTree &visitor) const noexcept override {
    return visitor.visit(this);
  }

  [[nodiscard]] std::expected<bool, Error>
  accept(const ValidateTree &visitor) const noexcept override {
    return visitor.visit(this);
  }
};

} // namespace nicole

#endif
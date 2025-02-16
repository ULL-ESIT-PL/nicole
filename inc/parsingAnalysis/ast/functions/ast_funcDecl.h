#ifndef AST_FUNC_DECL_H
#define AST_FUNC_DECL_H

#include "ast_parametrizedSubritineDecl.h"

namespace nicole {

class AST_FUNC_DECL final : public AST_PARAMETRIZED_SUBRUTINE_DECL {
private:
public:
  explicit AST_FUNC_DECL(const std::string &id, const Parameters &params,
                         const std::string &returnType,
                         const std::shared_ptr<AST_BODY> &body) noexcept
      : AST_PARAMETRIZED_SUBRUTINE_DECL(AST_TYPE::FUNC_DECL, id, params,
                                        returnType, body) {}

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
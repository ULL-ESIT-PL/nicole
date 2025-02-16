#ifndef AST_METHOD_DECL_H
#define AST_METHOD_DECL_H

#include "../functions/ast_parametrizedSubritineDecl.h"

namespace nicole {

class AST_METHOD_DECL final : public AST_PARAMETRIZED_SUBRUTINE_DECL {
private:
  bool isVirtual_;

public:
  explicit AST_METHOD_DECL(const std::string &id, const Parameters &params,
                           const std::string &returnType,
                           const std::shared_ptr<AST_BODY> &body,
                           const bool isVirtual) noexcept
      : AST_PARAMETRIZED_SUBRUTINE_DECL(AST_TYPE::METHOD_DECL, id, params,
                                        returnType, body),
        isVirtual_{isVirtual} {}

  [[nodiscard]] bool isVirtual() const noexcept { return isVirtual_; }

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
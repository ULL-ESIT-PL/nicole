#ifndef AST_AUTO_DECL_H
#define AST_AUTO_DECL_H

#include "ast_varDecl.h"

namespace nicole {

class AST_AUTO_DECL : public AST_VAR_DECL {
public:
  explicit AST_AUTO_DECL(const std::string &id,
                         const std::shared_ptr<AST> &value, const bool isConst) noexcept
      : AST_VAR_DECL(AST_TYPE::AUTO_DECL, id, value, isConst) {}

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
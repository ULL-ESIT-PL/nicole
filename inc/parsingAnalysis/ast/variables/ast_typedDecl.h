#ifndef AST_VAR_TYPED_DECL_H
#define AST_VAR_TYPED_DECL_H

#include "ast_varDecl.h"
#include "../../types/type.h"

namespace nicole {

class AST_VAR_TYPED_DECL : public AST_VAR_DECL {
private:
  std::string type_;

public:
  explicit AST_VAR_TYPED_DECL(const std::string &id, const std::string &type,
                        const std::shared_ptr<AST> &value, const bool isConst) noexcept
      : AST_VAR_DECL(AST_TYPE::VAR_TYPED_DECL, id, value, isConst), type_{type} {}

  [[nodiscard]] const std::string &varType() const noexcept { return type_; }

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
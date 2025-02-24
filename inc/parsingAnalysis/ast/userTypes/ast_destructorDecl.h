#ifndef AST_DESTRUCTOR_DECL_H
#define AST_DESTRUCTOR_DECL_H

#include "../../types/typeTable.h"
#include "../functions/ast_subrutineDecl.h"
#include <memory>

namespace nicole {

class AST_DESTRUCTOR_DECL final : public AST_SUBRUTINE_DECL {
public:
  explicit AST_DESTRUCTOR_DECL(const std::string &id,
                               const std::shared_ptr<AST_BODY> &body) noexcept
      : AST_SUBRUTINE_DECL(AST_TYPE::DESTRUCTOR_DECL, id,
                           std::make_shared<VoidType>(), body) {}

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
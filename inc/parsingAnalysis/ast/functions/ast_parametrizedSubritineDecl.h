#ifndef AST_PARAMETRIZED_SUBRUTINE_DECL_H
#define AST_PARAMETRIZED_SUBRUTINE_DECL_H

#include "ast_parameter.h"
#include "ast_subrutineDecl.h"
#include "parameters.h"
#include <memory>
#include <string>

namespace nicole {

class AST_PARAMETRIZED_SUBRUTINE_DECL : public AST_SUBRUTINE_DECL {
private:
  std::vector<std::shared_ptr<AST_PARAMETER>> params_;

public:
  explicit AST_PARAMETRIZED_SUBRUTINE_DECL(
      const AST_TYPE type, const std::string &id,
      const std::vector<std::shared_ptr<AST_PARAMETER>> &params,
      const std::shared_ptr<Type> &returnType,
      const std::shared_ptr<AST_BODY> &body) noexcept
      : AST_SUBRUTINE_DECL(type, id, returnType, body), params_{params} {}

  [[nodiscard]] const std::vector<std::shared_ptr<AST_PARAMETER>> &parameters() const noexcept {
    return params_;
  }
};

} // namespace nicole

#endif
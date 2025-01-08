#ifndef AST_BODY_H
#define AST_BODY_H

#include "ast_statement.h"
#include <memory>
#include <vector>

namespace nicole {

class AST_BODY : public AST {
private:
  std::vector<std::shared_ptr<AST_STATEMENT>> body_;

public:
  explicit AST_BODY(
      const std::vector<std::shared_ptr<AST_STATEMENT>> &body) noexcept
      : AST(AST_TYPE::BODY), body_{body} {}

  [[nodiscard]] const std::vector<std::shared_ptr<AST_STATEMENT>> &
  body() const noexcept {
    return body_;
  }
};

} // namespace nicole

#endif
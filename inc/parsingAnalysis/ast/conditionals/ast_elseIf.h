#ifndef AST_ELSE_IF_H
#define AST_ELSE_IF_H

#include "../statements/ast_body.h"
#include <memory>
#include <vector>

namespace nicole {

class AST_ELSE_IF : public AST {
private:
  std::shared_ptr<AST> condition_;
  std::shared_ptr<AST_BODY> body_;

public:
  explicit AST_ELSE_IF(const std::shared_ptr<AST> &condition,
                  const std::shared_ptr<AST_BODY> &body,
                  const SourceLocation &sourceLocation) noexcept
      : AST(AST_TYPE::ELSE_IF, sourceLocation), condition_{condition}, body_{body} {}

  [[nodiscard]] const std::shared_ptr<AST> &condition() const noexcept {
    return condition_;
  }

  [[nodiscard]] const std::shared_ptr<AST_BODY> &body() const noexcept {
    return body_;
  }
};

} // namespace nicole

#endif
#ifndef AST_WHILE_H
#define AST_WHILE_H

#include "../statements/ast_body.h"
#include <memory>

namespace nicole {

class AST_WHILE : public AST {
private:
  std::shared_ptr<AST> condition_;
  std::shared_ptr<AST_BODY> body_;

public:
  explicit AST_WHILE(const std::shared_ptr<AST> &condition,
                     const std::shared_ptr<AST_BODY> &body,
                     const SourceLocation &sourceLocation) noexcept
      : AST(AST_TYPE::WHILE, sourceLocation), condition_{condition},
        body_{body} {}

  [[nodiscard]] const std::shared_ptr<AST> &condition() const noexcept {
    return condition_;
  }

  [[nodiscard]] const std::shared_ptr<AST_BODY> &body() const noexcept {
    return body_;
  }
};

} // namespace nicole

#endif
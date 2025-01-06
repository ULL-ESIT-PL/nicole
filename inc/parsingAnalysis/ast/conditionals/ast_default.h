#ifndef AST_DEFAULT_H
#define AST_DEFAULT_H

#include "../statements/ast_body.h"
#include <memory>

namespace nicole {

class AST_DEFAULT : public AST {
private:
  std::shared_ptr<AST_BODY> body_;

public:
  explicit AST_DEFAULT(const std::shared_ptr<AST_BODY> &body,
                    const SourceLocation &sourceLocation) noexcept
      : AST(AST_TYPE::DEFAULT, sourceLocation), body_{body} {}

  [[nodiscard]] const std::shared_ptr<AST_BODY> &body() const noexcept {
    return body_;
  }
};

} // namespace nicole

#endif
#ifndef AST_CASE_H
#define AST_CASE_H

#include "../statements/ast_body.h"
#include <memory>

namespace nicole {

class AST_CASE : public AST {
private:
  std::shared_ptr<AST> match_;
  std::shared_ptr<AST_BODY> body_;

public:
  explicit AST_CASE(const std::shared_ptr<AST> &match,
                    const std::shared_ptr<AST_BODY> &body,
                    const SourceLocation &sourceLocation) noexcept
      : AST(AST_TYPE::CASE, sourceLocation), match_{match}, body_{body} {}

  [[nodiscard]] const std::shared_ptr<AST> &match() const noexcept {
    return match_;
  }

  [[nodiscard]] const std::shared_ptr<AST_BODY> &body() const noexcept {
    return body_;
  }
};

} // namespace nicole

#endif
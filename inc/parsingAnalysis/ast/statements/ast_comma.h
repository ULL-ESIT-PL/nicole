#ifndef AST_COMMA_H
#define AST_COMMA_H

#include "ast_statement.h"
#include <memory>
#include <vector>

namespace nicole {

class AST_COMMA : public AST {
private:
  std::vector<std::shared_ptr<AST_STATEMENT>> body_;

public:
  explicit AST_COMMA(const std::vector<std::shared_ptr<AST_STATEMENT>> &body,
                     const SourceLocation &sourceLocation) noexcept
      : AST(AST_TYPE::COMMA, sourceLocation), body_{body} {}

  [[nodiscard]] const std::vector<std::shared_ptr<AST_STATEMENT>> &
  body() const noexcept {
    return body_;
  }
};

} // namespace nicole

#endif
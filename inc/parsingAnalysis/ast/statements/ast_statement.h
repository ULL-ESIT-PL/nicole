#ifndef AST_STATEMENT_H
#define AST_STATEMENT_H

#include "../ast.h"
#include <memory>

namespace nicole {

class AST_STATEMENT : public AST {
private:
  std::shared_ptr<AST> expression_;

public:
  explicit AST_STATEMENT(const std::shared_ptr<AST> &expression) noexcept
      : AST(AST_TYPE::STATEMENT), expression_{expression} {}

  [[nodiscard]] const std::shared_ptr<AST> &expression() const noexcept {
    return expression_;
  }
};

} // namespace nicole

#endif
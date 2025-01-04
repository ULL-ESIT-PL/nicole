#ifndef AST_RETURN_H
#define AST_RETURN_H

#include "../ast.h"
#include <memory>

namespace nicole {

class AST_RETURN : public AST {
private:
  std::shared_ptr<AST> expression_;

public:
  explicit AST_RETURN(const std::shared_ptr<AST> &expression,
                   const SourceLocation &sourceLocation) noexcept
      : AST(AST_TYPE::RETURN, sourceLocation), expression_{expression} {}

  [[nodiscard]] const std::shared_ptr<AST> &expression() const noexcept {
    return expression_;
  }
};

} // namespace nicole

#endif
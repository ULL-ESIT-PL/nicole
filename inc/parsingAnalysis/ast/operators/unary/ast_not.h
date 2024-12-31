#ifndef AST_NOT_H
#define AST_NOT_H

#include "../../../../lexicalAnalysis/token.h"
#include "../../ast.h"
#include <memory>

namespace nicole {

class AST_NOT : public AST {
private:
  Token op_;
  std::shared_ptr<AST> value_;

public:
  explicit AST_NOT(const Token &op, const std::shared_ptr<AST> &value,
                   const SourceLocation &sourceLocation) noexcept
      : AST(AST_TYPE::NOT, sourceLocation), op_{op}, value_{value} {}

  [[nodiscard]] Token op() const noexcept { return op_; }

  [[nodiscard]] const std::shared_ptr<AST> &value() const noexcept {
    return value_;
  }
};

} // namespace nicole

#endif
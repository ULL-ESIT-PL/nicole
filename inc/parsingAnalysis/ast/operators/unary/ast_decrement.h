#ifndef AST_DECREMENT_H
#define AST_DECREMENT_H

#include "../../../../lexicalAnalysis/token.h"
#include "../../ast.h"
#include <memory>

namespace nicole {

class AST_DECREMENT : public AST {
private:
  Token op_;
  std::shared_ptr<AST> value_;

public:
  explicit AST_DECREMENT(const Token &op, const std::shared_ptr<AST> &value,
                         const SourceLocation &sourceLocation) noexcept
      : AST(AST_TYPE::DECREMENT, sourceLocation), op_{op}, value_{value} {}

  [[nodiscard]] const Token &op() const noexcept { return op_; }

  [[nodiscard]] const std::shared_ptr<AST> &value() const noexcept {
    return value_;
  }
};

} // namespace nicole

#endif
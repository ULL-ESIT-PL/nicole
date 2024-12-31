#ifndef AST_INCREMENT_H
#define AST_INCREMENT_H

#include "../../../../lexicalAnalysis/token.h"
#include "../../ast.h"
#include <memory>

namespace nicole {

class AST_INCREMENT : public AST {
private:
  Token op_;
  std::shared_ptr<AST> value_;

public:
  explicit AST_INCREMENT(const Token &op, const std::shared_ptr<AST> &value,
                         const SourceLocation &sourceLocation) noexcept
      : AST(AST_TYPE::INCREMENT, sourceLocation), op_{op}, value_{value} {}

  [[nodiscard]] Token op() const noexcept { return op_; }

  [[nodiscard]] const std::shared_ptr<AST> &value() const noexcept {
    return value_;
  }
};

} // namespace nicole

#endif
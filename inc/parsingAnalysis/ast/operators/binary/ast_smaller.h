#ifndef AST_SMALLER_H
#define AST_SMALLER_H

#include "../../../../lexicalAnalysis/token.h"
#include "../../ast.h"
#include <memory>

namespace nicole {

class AST_SMALLER : public AST {
private:
  Token op_;
  std::shared_ptr<AST> left_;
  std::shared_ptr<AST> right_;

public:
  explicit AST_SMALLER(const Token &op, const std::shared_ptr<AST> &left,
                   const std::shared_ptr<AST> &right,
                   const SourceLocation &sourceLocation) noexcept
      : AST(AST_TYPE::SMALLER, sourceLocation), op_{op}, left_{left},
        right_{right} {}

  [[nodiscard]] Token op() const noexcept { return op_; }

  [[nodiscard]] const std::shared_ptr<AST> &left() const noexcept {
    return left_;
  }

  [[nodiscard]] const std::shared_ptr<AST> &right() const noexcept {
    return right_;
  }
};

} // namespace nicole

#endif
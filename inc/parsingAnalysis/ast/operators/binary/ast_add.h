#ifndef AST_ADD_H
#define AST_ADD_H

#include "../../ast.h"
#include <memory>

namespace nicole {

enum class OPERATOR : unsigned {
  ADD = static_cast<unsigned>(AST_TYPE::ADD),
};

class AST_ADD : public AST {
private:
  OPERATOR op_{OPERATOR::ADD};
  std::shared_ptr<AST> left_;
  std::shared_ptr<AST> right_;

public:
  explicit AST_ADD(const std::shared_ptr<AST>& left,
                   const std::shared_ptr<AST>& right,
                   const SourceLocation &sourceLocation) noexcept
      : AST(AST_TYPE::ADD, sourceLocation), left_{left}, right_{right} {
  }

  [[nodiscard]] OPERATOR op() const noexcept { return op_; }

  [[nodiscard]] const std::shared_ptr<AST>& left() const noexcept { return left_; }

  [[nodiscard]] const std::shared_ptr<AST>& right() const noexcept { return right_; }
};

} // namespace nicole

#endif
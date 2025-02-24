#ifndef AST_ASSIGNMENT_H
#define AST_ASSIGNMENT_H

#include "../../../lexicalAnalysis/token.h"
#include "../ast.h"
#include <string>
#include <variant>

namespace nicole {

class AST_ASSIGNMENT final : public AST {
private:
  Token op_;
  std::shared_ptr<AST> left_;
  std::shared_ptr<AST> value_;

public:
  explicit AST_ASSIGNMENT(const Token &op,
                               const std::shared_ptr<AST> &left,
                               const std::shared_ptr<AST> &value) noexcept
      : AST(AST_TYPE::ASSIGNMENT), op_{op}, left_{left}, value_{value} {}

  [[nodiscard]] const Token &op() const noexcept { return op_; }

  [[nodiscard]] const std::shared_ptr<AST> &left() const noexcept {
    return left_;
  }

  [[nodiscard]] const std::shared_ptr<AST> &value() const noexcept {
    return value_;
  }

  [[nodiscard]] std::expected<std::string, Error>
  accept(const PrintTree &visitor) const noexcept override {
    return visitor.visit(this);
  }

  [[nodiscard]] std::expected<bool, Error>
  accept(const ValidateTree &visitor) const noexcept override {
    return visitor.visit(this);
  }

  [[nodiscard]] std::expected<std::monostate, Error>
  accept(const FillScopes &visitor) const noexcept override {
    return visitor.visit(this);
  }
};

} // namespace nicole

#endif
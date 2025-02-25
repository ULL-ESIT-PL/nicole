#ifndef AST_UNARY_H
#define AST_UNARY_H

#include "../../../lexicalAnalysis/token.h"
#include "../ast.h"
#include <memory>

namespace nicole {

class AST_UNARY : public AST {
private:
  Token op_;
  std::shared_ptr<AST> value_;

public:
  explicit AST_UNARY(const Token &op, const std::shared_ptr<AST> &value) noexcept
      : AST(AST_TYPE::UNARY), op_{op}, value_{value} {}

  [[nodiscard]] const Token &op() const noexcept { return op_; }

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
  accept(const FillSemanticInfo &visitor) const noexcept override {
    return visitor.visit(this);
  }
};

} // namespace nicole

#endif
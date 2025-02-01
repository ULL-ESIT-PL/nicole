#ifndef AST_SELF_MULT_H
#define AST_SELF_MULT_H

#include "../ast.h"
#include <string>

namespace nicole {

class AST_SELF_MULT final : public AST {
private:
  std::shared_ptr<AST> left_;
  std::shared_ptr<AST> value_;

public:
  explicit AST_SELF_MULT(const std::shared_ptr<AST> &left,
                          const std::shared_ptr<AST> &value) noexcept
      : AST(AST_TYPE::SELF_MULT), left_{left}, value_{value} {}

  [[nodiscard]] const std::shared_ptr<AST> &left() const noexcept { return left_; }

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
};

} // namespace nicole

#endif
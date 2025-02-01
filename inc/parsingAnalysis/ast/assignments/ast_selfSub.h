#ifndef AST_SELF_SUB_H
#define AST_SELF_SUB_H

#include "../ast.h"
#include <string>

namespace nicole {

class AST_SELF_SUB final : public AST {
private:
  std::shared_ptr<AST> left_;
  std::shared_ptr<AST> value_;

public:
  explicit AST_SELF_SUB(const std::shared_ptr<AST> &left,
                        const std::shared_ptr<AST> &value) noexcept
      : AST(AST_TYPE::SELF_SUB), left_{left}, value_{value} {}

  [[nodiscard]] const std::shared_ptr<AST> &left() const noexcept { return left_; }

  [[nodiscard]] const std::shared_ptr<AST> &value() const noexcept {
    return value_;
  }

  [[nodiscard]] std::expected<std::string, Error>
  accept(const PrintTree &visitor) const noexcept override {
    return visitor.visit(this);
  }
};

} // namespace nicole

#endif
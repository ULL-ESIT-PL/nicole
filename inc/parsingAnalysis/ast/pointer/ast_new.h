#ifndef AST_NEW_H
#define AST_NEW_H

#include "../ast.h"
#include <memory>

namespace nicole {

class AST_NEW : public AST {
private:
  std::shared_ptr<AST> value_;

public:
  explicit AST_NEW(const std::shared_ptr<AST> &value) noexcept
      : AST(AST_TYPE::NEW), value_{value} {}

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
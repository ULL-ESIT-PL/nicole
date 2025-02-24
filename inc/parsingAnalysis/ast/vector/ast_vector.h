#ifndef AST_VECTOR_H
#define AST_VECTOR_H

#include "../ast.h"
#include <memory>
#include <vector>

namespace nicole {

class AST_VECTOR : public AST {
private:
  std::vector<std::shared_ptr<AST>> values_;

public:
  explicit AST_VECTOR(const std::vector<std::shared_ptr<AST>> &values) noexcept
      : AST(AST_TYPE::VECTOR), values_{values} {}

  [[nodiscard]] const std::vector<std::shared_ptr<AST>> &
  values() const noexcept {
    return values_;
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
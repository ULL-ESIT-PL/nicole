#ifndef AST_PRINT_H
#define AST_PRINT_H

#include "../statements/ast_comma.h"
#include <memory>

namespace nicole {

class AST_PRINT : public AST {
private:
  std::vector<std::shared_ptr<AST>> values_;

public:
  explicit AST_PRINT(const std::vector<std::shared_ptr<AST>> &values) noexcept
      : AST(AST_TYPE::PRINT), values_{values} {}

  [[nodiscard]] const std::vector<std::shared_ptr<AST>> &
  values() const noexcept {
    return values_;
  }

  [[nodiscard]] std::expected<std::string, Error>
  accept(const PrintTree &visitor) const noexcept override {
    return visitor.visit(this);
  }
};

} // namespace nicole

#endif
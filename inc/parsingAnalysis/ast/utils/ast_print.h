#ifndef AST_PRINT_H
#define AST_PRINT_H

#include "../statements/ast_comma.h"
#include <memory>

namespace nicole {

class AST_PRINT : public AST {
private:
  std::shared_ptr<AST_COMMA> values_;

public:
  explicit AST_PRINT(const std::shared_ptr<AST_COMMA> &values) noexcept
      : AST(AST_TYPE::PRINT), values_{values} {}

  [[nodiscard]] const std::shared_ptr<AST_COMMA> &values() const noexcept {
    return values_;
  }

  [[nodiscard]] std::expected<std::string, Error>
  accept(const PrintTree &visitor) const noexcept override {
    return visitor.visit(this);
  }
};

} // namespace nicole

#endif
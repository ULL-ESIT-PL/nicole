#ifndef AST_ENUM_H
#define AST_ENUM_H

#include "../ast.h"
#include <string>
#include <vector>

namespace nicole {

class AST_ENUM final : public AST {
private:
  std::vector<std::string> enumIdentifiers_;

public:
  explicit AST_ENUM(const std::vector<std::string> &enumIdentifiers) noexcept
      : AST(AST_TYPE::ENUM), enumIdentifiers_{enumIdentifiers} {}

  [[nodiscard]] const std::vector<std::string> &identifiers() const noexcept {
    return enumIdentifiers_;
  }

  [[nodiscard]] std::expected<std::string, Error>
  accept(const PrintTree &visitor) const noexcept override {
    return visitor.visit(this);
  }
};

} // namespace nicole

#endif
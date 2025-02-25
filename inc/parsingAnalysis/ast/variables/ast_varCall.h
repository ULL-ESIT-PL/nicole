#ifndef AST_VAR_CALL_H
#define AST_VAR_CALL_H

#include "../ast.h"
#include <string>

namespace nicole {

class AST_VAR_CALL final : public AST {
private:
  std::string id_;

public:
  explicit AST_VAR_CALL(const std::string &id) noexcept
      : AST(AST_TYPE::VAR_CALL), id_{id} {}

  [[nodiscard]] const std::string &id() const noexcept { return id_; }

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
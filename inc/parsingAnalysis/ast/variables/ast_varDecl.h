#ifndef AST_VAR_DECL_H
#define AST_VAR_DECL_H

#include "../ast.h"
#include <variant>

namespace nicole {

class AST_VAR_DECL : public AST {
private:
  std::string id_;
  std::shared_ptr<AST> value_;
  bool isConst_;

public:
  explicit AST_VAR_DECL(const AST_TYPE astType, const std::string &id,
                        const std::shared_ptr<AST> &value,
                        const bool isConst) noexcept
      : AST(astType), id_{id}, value_{value}, isConst_{isConst} {}

  [[nodiscard]] const std::string &id() const noexcept { return id_; }

  [[nodiscard]] const std::shared_ptr<AST> &value() const noexcept {
    return value_;
  }

  [[nodiscard]] bool isConst() const noexcept { return isConst_; }

  [[nodiscard]] virtual std::expected<std::string, Error>
  accept(const PrintTree &visitor) const noexcept = 0;

  [[nodiscard]] virtual std::expected<bool, Error>
  accept(const ValidateTree &visitor) const noexcept = 0;

  [[nodiscard]] virtual std::expected<std::monostate, Error>
  accept(const FillScopes &visitor) const noexcept = 0;
};

} // namespace nicole

#endif
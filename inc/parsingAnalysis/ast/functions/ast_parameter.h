#ifndef AST_PARAMETER_H
#define AST_PARAMETER_H

#include "../../../tables/typeTable/types/type.h"
#include "../ast.h"
#include <memory>
#include <string>

namespace nicole {

class AST_PARAMETER final : public AST {
private:
  std::string id_;
  std::shared_ptr<Type> varType_;

public:
  explicit AST_PARAMETER(const std::string &id,
                         const std::shared_ptr<Type> &varType) noexcept
      : AST{AST_TYPE::PARAMETER}, id_{id}, varType_{varType} {}

  [[nodiscard]] const std::string &id() const noexcept { return id_; }

  [[nodiscard]] const std::shared_ptr<Type> &varType() const noexcept {
    return varType_;
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
#ifndef AST_THIS_H
#define AST_THIS_H

#include "../../../tables/typeTable/types/userTypes/userType.h"
#include "../ast.h"
#include <memory>

namespace nicole {

class AST_THIS final : public AST {
private:
  mutable std::shared_ptr<UserType> userType_{nullptr};

public:
  explicit AST_THIS(const long long unsigned nodeId) noexcept
      : AST{nodeId, AST_TYPE::THIS} {}

  ~AST_THIS() noexcept = default;

  [[nodiscard]] const std::shared_ptr<UserType> &userType() const noexcept {
    return userType_;
  }

  void setUserType(const std::shared_ptr<UserType> &userType) const noexcept {
    userType_ = userType;
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

  [[nodiscard]] std::expected<std::shared_ptr<Type>, Error>
  accept(const TypeAnalysis &visitor) const noexcept override {
    return visitor.visit(this);
  }

  [[nodiscard]] std::expected<std::monostate, Error>
  accept(const Monomorphize &visitor) const noexcept override {
    return visitor.visit(this);
  }

  [[nodiscard]] std::expected<std::shared_ptr<llvm::Value>, Error>
  accept(const CodeGeneration &visitor) const noexcept override {
    return visitor.visit(this);
  }
};

} // namespace nicole

#endif // AST_H

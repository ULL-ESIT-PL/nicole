#ifndef AST_ENUM_ACCESS_H
#define AST_ENUM_ACCESS_H

#include "../ast.h"
#include <string>

namespace nicole {

class AST_ENUM_ACCESS final : public AST {
private:
  std::string enumId_;
  std::string identifier_;

public:
  explicit AST_ENUM_ACCESS(const std::string &enumId,
                           const std::string &identifier) noexcept
      : AST{AST_TYPE::ENUM_ACCESS}, enumId_{enumId}, identifier_{identifier} {}

  [[nodiscard]] const std::string &enumId() const noexcept { return enumId_; }

  [[nodiscard]] const std::string &identifier() const noexcept {
    return identifier_;
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

#endif
#ifndef AST_ENUM_H
#define AST_ENUM_H

#include "../ast.h"
#include <algorithm>
#include <string>
#include <vector>

namespace nicole {

class AST_ENUM final : public AST {
private:
  std::string id_;
  std::vector<std::string> enumIdentifiers_;

public:
  explicit AST_ENUM(const std::string &id,
                    const std::vector<std::string> &enumIdentifiers) noexcept
      : AST(AST_TYPE::ENUM), id_{id}, enumIdentifiers_{enumIdentifiers} {}

  [[nodiscard]] const std::string &id() const noexcept { return id_; }

  [[nodiscard]] const std::vector<std::string> &identifiers() const noexcept {
    return enumIdentifiers_;
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
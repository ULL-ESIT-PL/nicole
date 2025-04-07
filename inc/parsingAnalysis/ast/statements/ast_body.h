#ifndef AST_BODY_H
#define AST_BODY_H

#include "../../../tables/scope/scope.h"
#include "ast_statement.h"
#include <memory>
#include <vector>

namespace nicole {

class AST_BODY : public AST {
private:
  std::vector<std::shared_ptr<AST_STATEMENT>> body_;
  mutable std::shared_ptr<Scope> scope_;

public:
  explicit AST_BODY(
      const long long unsigned nodeId,
      const std::vector<std::shared_ptr<AST_STATEMENT>> &body) noexcept
      : AST(nodeId, AST_TYPE::BODY), body_{body} {}

  [[nodiscard]] const std::vector<std::shared_ptr<AST_STATEMENT>> &
  body() const noexcept {
    return body_;
  }

  [[nodiscard]] const std::shared_ptr<Scope> &scope() const noexcept {
    return scope_;
  }

  void setScope(const std::shared_ptr<Scope> &scope) const noexcept {
    scope_ = scope;
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
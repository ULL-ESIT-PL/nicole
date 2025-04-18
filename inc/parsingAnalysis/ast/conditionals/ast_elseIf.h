#ifndef AST_ELSE_IF_H
#define AST_ELSE_IF_H

#include "../statements/ast_body.h"
#include "ast_condition.h"
#include <memory>

namespace nicole {

class AST_ELSE_IF : public AST {
private:
  std::shared_ptr<AST_CONDITION> condition_;
  std::shared_ptr<AST_BODY> body_;

public:
  explicit AST_ELSE_IF(const long long unsigned nodeId,
                       const SourceLocation &srcLoc,
                       const std::shared_ptr<AST_CONDITION> &condition,
                       const std::shared_ptr<AST_BODY> &body) noexcept
      : AST(nodeId, AST_TYPE::ELSE_IF, srcLoc), condition_{condition},
        body_{body} {}

  [[nodiscard]] const std::shared_ptr<AST_CONDITION> &
  condition() const noexcept {
    return condition_;
  }

  [[nodiscard]] const std::shared_ptr<AST_BODY> &body() const noexcept {
    return body_;
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
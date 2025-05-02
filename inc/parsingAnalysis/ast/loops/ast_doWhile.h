#ifndef AST_DO_WHILE_H
#define AST_DO_WHILE_H

#include "../conditionals/ast_condition.h"
#include "ast_loop.h"
#include <memory>

namespace nicole {

class AST_DO_WHILE : public AST_LOOP {
private:
  std::shared_ptr<AST_CONDITION> condition_;

public:
  explicit AST_DO_WHILE(
      const long long unsigned nodeId, const SourceLocation &srcLoc,
      const std::shared_ptr<AST_BODY> &body,
      const std::shared_ptr<AST_CONDITION> &condition) noexcept
      : AST_LOOP{nodeId, AST_TYPE::DO_WHILE, srcLoc, condition, body} {}

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

  [[nodiscard]] std::expected<llvm::Value*, Error>
  accept(const CodeGeneration &visitor) const noexcept override {
    return visitor.visit(this);
  }
};

} // namespace nicole

#endif
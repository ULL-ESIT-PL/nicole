#ifndef AST_FOR_H
#define AST_FOR_H

#include "ast_loop.h"
#include <memory>

namespace nicole {

class AST_FOR : public AST_LOOP {
private:
  std::vector<std::shared_ptr<AST>> init_;
  std::shared_ptr<AST_CONDITION> condition_;
  std::vector<std::shared_ptr<AST>> update_;

public:
  explicit AST_FOR(const std::vector<std::shared_ptr<AST>> &init,
                   const std::shared_ptr<AST_CONDITION> &condition,
                   const std::vector<std::shared_ptr<AST>> &update,
                   const std::shared_ptr<AST_BODY> &body) noexcept
      : AST_LOOP(AST_TYPE::FOR, condition, body), init_{init}, update_{update} {
  }

  [[nodiscard]] const std::vector<std::shared_ptr<AST>> &init() const noexcept {
    return init_;
  }

  [[nodiscard]] const std::vector<std::shared_ptr<AST>> &
  update() const noexcept {
    return update_;
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
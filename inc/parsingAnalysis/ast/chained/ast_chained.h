#ifndef AST_CHAINED_H
#define AST_CHAINED_H

#include "../ast.h"
#include <memory>
#include <vector>

namespace nicole {

class AST_CHAINED final : public AST {
private:
  std::shared_ptr<AST> base_;
  std::vector<std::shared_ptr<AST>> operations_;

public:
  explicit AST_CHAINED(
      const long long unsigned nodeId, const SourceLocation &srcLoc,
      const std::shared_ptr<AST> &base,
      const std::vector<std::shared_ptr<AST>> &operations) noexcept
      : AST{nodeId, AST_TYPE::CHAIN, srcLoc}, base_{base},
        operations_{operations} {}

  [[nodiscard]] const std::shared_ptr<AST> &base() const noexcept {
    return base_;
  }

  [[nodiscard]] const std::vector<std::shared_ptr<AST>> &
  operations() const noexcept {
    return operations_;
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

  [[nodiscard]] std::expected<llvm::Value*, Error>
  accept(const CodeGeneration &visitor) const noexcept override {
    return visitor.visit(this);
  }
};

} // namespace nicole

#endif // AST_H

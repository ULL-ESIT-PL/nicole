#ifndef AST_PASS_H
#define AST_PASS_H

#include "../ast.h"
#include <memory>

namespace nicole {

class AST_PASS : public AST {
private:
  std::shared_ptr<AST> fatherLoop_;

public:
  explicit AST_PASS(const long long unsigned nodeId,
                    const SourceLocation &srcLoc,
                    const std::shared_ptr<AST> &fatherLoop) noexcept
      : AST(nodeId, AST_TYPE::PASS, srcLoc), fatherLoop_{fatherLoop} {}

  [[nodiscard]] const std::shared_ptr<AST> &fatherLoop() const noexcept {
    return fatherLoop_;
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
#ifndef AST_RETURN_H
#define AST_RETURN_H

#include "../ast.h"
#include <memory>

namespace nicole {

class AST_RETURN : public AST {
private:
  std::shared_ptr<AST> expression_;

public:
  explicit AST_RETURN(const long long unsigned nodeId,
                      const SourceLocation &srcLoc,
                      const std::shared_ptr<AST> &expression) noexcept
      : AST(nodeId, AST_TYPE::RETURN, srcLoc), expression_{expression} {}

  [[nodiscard]] const std::shared_ptr<AST> &expression() const noexcept {
    return expression_;
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

#endif
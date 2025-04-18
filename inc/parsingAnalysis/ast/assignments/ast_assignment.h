#ifndef AST_ASSIGNMENT_H
#define AST_ASSIGNMENT_H

#include "../../../lexicalAnalysis/token.h"
#include "../ast.h"
#include <string>

namespace nicole {

class AST_ASSIGNMENT final : public AST {
private:
  Token op_;
  std::shared_ptr<AST> left_;
  std::shared_ptr<AST> value_;

public:
  explicit AST_ASSIGNMENT(const long long unsigned nodeId, const SourceLocation &srcLoc, const Token &op,
                          const std::shared_ptr<AST> &left,
                          const std::shared_ptr<AST> &value) noexcept
      : AST(nodeId, AST_TYPE::ASSIGNMENT, srcLoc), op_{op}, left_{left}, value_{value} {
  }

  [[nodiscard]] const Token &op() const noexcept { return op_; }

  [[nodiscard]] const std::shared_ptr<AST> &left() const noexcept {
    return left_;
  }

  [[nodiscard]] const std::shared_ptr<AST> &value() const noexcept {
    return value_;
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
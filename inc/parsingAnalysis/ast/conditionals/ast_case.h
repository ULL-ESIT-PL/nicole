#ifndef AST_CASE_H
#define AST_CASE_H

#include "../statements/ast_body.h"
#include <memory>

namespace nicole {

class AST_CASE : public AST {
private:
  std::shared_ptr<AST> match_;
  std::shared_ptr<AST_BODY> body_;

public:
  explicit AST_CASE(const std::shared_ptr<AST> &match,
                    const std::shared_ptr<AST_BODY> &body) noexcept
      : AST(AST_TYPE::CASE), match_{match}, body_{body} {}

  [[nodiscard]] const std::shared_ptr<AST> &match() const noexcept {
    return match_;
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
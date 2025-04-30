#ifndef AST_CONSTRUCTOR_CALL_H
#define AST_CONSTRUCTOR_CALL_H

#include "../../../tables/typeTable/types/type.h"
#include "../functions/ast_callable.h"
#include <string>
#include <vector>

namespace nicole {

class AST_CONSTRUCTOR_CALL final : public AST_CALLABLE {
private:
  mutable std::vector<std::shared_ptr<Type>> replaceOfGenerics_;
  mutable std::vector<std::shared_ptr<AST>> parameters_;

public:
  explicit AST_CONSTRUCTOR_CALL(
      const long long unsigned nodeId, const SourceLocation &srcLoc,
      const std::string &id,
      const std::vector<std::shared_ptr<Type>> &replaceOfGenerics,
      const std::vector<std::shared_ptr<AST>> &parameters) noexcept
      : AST_CALLABLE(nodeId, AST_TYPE::CONSTRUCTOR_CALL, srcLoc, id),
        replaceOfGenerics_{replaceOfGenerics}, parameters_{parameters} {}

  [[nodiscard]] const std::vector<std::shared_ptr<Type>> &
  replaceOfGenerics() const noexcept {
    return replaceOfGenerics_;
  }

  void setReplacesOfGenerics(
      const std::vector<std::shared_ptr<Type>> &replaces) const noexcept {
    replaceOfGenerics_ = replaces;
  }

  [[nodiscard]] const std::vector<std::shared_ptr<AST>> &
  parameters() const noexcept {
    return parameters_;
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
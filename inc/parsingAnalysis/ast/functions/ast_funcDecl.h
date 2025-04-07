#ifndef AST_FUNC_DECL_H
#define AST_FUNC_DECL_H

#include "../../../tables/typeTable/types/userTypes/genericParameter.h"
#include "ast_parametrizedSubritineDecl.h"

namespace nicole {

class AST_FUNC_DECL final : public AST_PARAMETRIZED_SUBRUTINE_DECL {
private:
  mutable std::vector<GenericParameter> generics_;

public:
  explicit AST_FUNC_DECL(const long long unsigned nodeId, const std::string &id,
                         const std::vector<GenericParameter> &generics,
                         const Parameters &params,
                         const std::shared_ptr<Type> &returnType,
                         const std::shared_ptr<AST_BODY> &body) noexcept
      : AST_PARAMETRIZED_SUBRUTINE_DECL(nodeId, AST_TYPE::FUNC_DECL, id, params,
                                        returnType, body),
        generics_{generics} {}

  [[nodiscard]] const std::vector<GenericParameter> &generics() const noexcept {
    return generics_;
  }

  void
  setGenerics(const std::vector<GenericParameter> &generics) const noexcept {
    generics_ = generics;
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
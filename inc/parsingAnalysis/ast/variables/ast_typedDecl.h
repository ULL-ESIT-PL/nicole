#ifndef AST_VAR_TYPED_DECL_H
#define AST_VAR_TYPED_DECL_H

#include "../../../tables/typeTable/types/type.h"
#include "ast_varDecl.h"
#include <memory>

namespace nicole {

class AST_VAR_TYPED_DECL : public AST_VAR_DECL {
private:
  mutable std::shared_ptr<Type> type_;

public:
  explicit AST_VAR_TYPED_DECL(const long long unsigned nodeId,
                              const std::string &id,
                              const std::shared_ptr<Type> &type,
                              const std::shared_ptr<AST> &value) noexcept
      : AST_VAR_DECL(nodeId, AST_TYPE::VAR_TYPED_DECL, id, value), type_{type} {
  }

  [[nodiscard]] const std::shared_ptr<Type> &varType() const noexcept {
    return type_;
  }

  void setVarType(const std::shared_ptr<Type> &type) const noexcept {
    type_ = type;
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
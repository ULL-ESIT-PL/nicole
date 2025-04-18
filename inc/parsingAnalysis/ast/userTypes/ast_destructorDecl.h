#ifndef AST_DESTRUCTOR_DECL_H
#define AST_DESTRUCTOR_DECL_H

#include "../../../tables/typeTable/types/specialTypes/voidType.h"
#include "../functions/ast_subrutineDecl.h"
#include <memory>

namespace nicole {

class AST_DESTRUCTOR_DECL final : public AST_SUBRUTINE_DECL {
public:
  explicit AST_DESTRUCTOR_DECL(const long long unsigned nodeId,
                               const SourceLocation &srcLoc,
                               const std::string &id,
                               const std::shared_ptr<AST_BODY> &body) noexcept
      : AST_SUBRUTINE_DECL(nodeId, AST_TYPE::DESTRUCTOR_DECL, srcLoc, id,
                           std::make_shared<VoidType>(), body) {}

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
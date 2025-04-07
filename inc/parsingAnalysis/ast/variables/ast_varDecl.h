#ifndef AST_VAR_DECL_H
#define AST_VAR_DECL_H

#include "../../../tables/scope/scope.h"
#include "../ast.h"
#include <memory>
#include <variant>

namespace nicole {

class AST_VAR_DECL : public AST {
private:
  std::string id_;
  std::shared_ptr<AST> value_;
  mutable std::shared_ptr<Scope> scope_;

public:
  explicit AST_VAR_DECL(const long long unsigned nodeId, const AST_TYPE astType,
                        const std::string &id,
                        const std::shared_ptr<AST> &value) noexcept
      : AST(nodeId, astType), id_{id}, value_{value} {}

  [[nodiscard]] const std::string &id() const noexcept { return id_; }

  [[nodiscard]] const std::shared_ptr<AST> &value() const noexcept {
    return value_;
  }

  [[nodiscard]] const std::shared_ptr<Scope> &scope() const noexcept {
    return scope_;
  }

  void setScope(const std::shared_ptr<Scope> &scope) const noexcept {
    scope_ = scope;
  }
};

} // namespace nicole

#endif
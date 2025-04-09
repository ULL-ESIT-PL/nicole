#include "../../../inc/visitors/monomorphize/monomorphize.h"
#include "../../../inc/parsingAnalysis/ast/variables/ast_autoDecl.h"
#include "../../../inc/parsingAnalysis/ast/variables/ast_typedDecl.h"
#include "../../../inc/parsingAnalysis/ast/variables/ast_varCall.h"
#include <variant>

namespace nicole {

std::expected<std::monostate, Error>
Monomorphize::visit(const AST_AUTO_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_AUTO_DECL");
  }
  const auto result{node->value()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  return {};
}

std::expected<std::monostate, Error>
Monomorphize::visit(const AST_VAR_TYPED_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_VAR_TYPED_DECL");
  }
  const auto result{node->value()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  return {};
}

std::expected<std::monostate, Error>
Monomorphize::visit(const AST_VAR_CALL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_VAR_CALL");
  }
  return {};
}

}
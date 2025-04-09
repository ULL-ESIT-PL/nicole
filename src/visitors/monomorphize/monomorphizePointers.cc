#include "../../../inc/visitors/monomorphize/monomorphize.h"
#include "../../../inc/parsingAnalysis/ast/pointer/ast_delete.h"
#include "../../../inc/parsingAnalysis/ast/pointer/ast_deref.h"
#include "../../../inc/parsingAnalysis/ast/pointer/ast_new.h"
#include <variant>

namespace nicole {

std::expected<std::monostate, Error>
Monomorphize::visit(const AST_DELETE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DELETE");
  }
  const auto result{node->value()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  return {};
}

std::expected<std::monostate, Error>
Monomorphize::visit(const AST_NEW *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_NEW");
  }
  const auto result{node->value()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  return {};
}

std::expected<std::monostate, Error>
Monomorphize::visit(const AST_DEREF *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DEREF");
  }
  const auto result{node->value()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  return {};
}

}
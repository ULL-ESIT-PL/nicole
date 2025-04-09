#include "../../../inc/visitors/monomorphize/monomorphize.h"
#include "../../../inc/parsingAnalysis/ast/vector/ast_index.h"
#include "../../../inc/parsingAnalysis/ast/vector/ast_vector.h"
#include <variant>

namespace nicole {

std::expected<std::monostate, Error>
Monomorphize::visit(const AST_VECTOR *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_VECTOR");
  }
  for (const auto &value : node->values()) {
    const auto result{value->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }
  return {};
}

std::expected<std::monostate, Error>
Monomorphize::visit(const AST_INDEX *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_INDEX");
  }
  const auto result{node->index()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  return {};
}

}
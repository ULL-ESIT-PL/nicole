#include "../../../inc/visitors/monomorphize/monomorphize.h"
#include "../../../inc/parsingAnalysis/ast/chained/ast_chained.h"
#include <variant>

namespace nicole {

std::expected<std::monostate, Error>
Monomorphize::visit(const AST_CHAINED *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_CHAINED");
  }
  const auto base{node->base()->accept(*this)};
  if (!base) {
    return createError(base.error());
  }
  for (const auto &chain : node->operations()) {
    const auto result{chain->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }
  return {};
}

}
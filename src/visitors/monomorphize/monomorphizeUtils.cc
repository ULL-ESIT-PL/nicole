#include "../../../inc/visitors/monomorphize/monomorphize.h"
#include "../../../inc/parsingAnalysis/ast/utils/ast_import.h"
#include "../../../inc/parsingAnalysis/ast/utils/ast_print.h"
#include <variant>

namespace nicole {

std::expected<std::monostate, Error>
Monomorphize::visit(const AST_PRINT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_PRINT");
  }
  for (const auto &chain : node->values()) {
    const auto result{chain->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }
  return {};
}

std::expected<std::monostate, Error>
Monomorphize::visit(const AST_IMPORT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_IMPORT");
  }
  return {};
}

}
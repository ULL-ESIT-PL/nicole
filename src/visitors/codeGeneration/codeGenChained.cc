#include "../../../inc/visitors/codeGeneration/codeGeneration.h"

#include "../../../inc/parsingAnalysis/ast/chained/ast_chained.h"

#include <memory>

namespace nicole {

std::expected<std::shared_ptr<llvm::Value>, Error>
CodeGeneration::visit(const AST_CHAINED *node) const noexcept {
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

} // namespace nicole
#include "../../../inc/visitors/codeGeneration/codeGeneration.h"

#include "../../../inc/parsingAnalysis/ast/vector/ast_index.h"
#include "../../../inc/parsingAnalysis/ast/vector/ast_vector.h"

#include <cstddef>
#include <memory>
#include <variant>

namespace nicole {

std::expected<std::shared_ptr<llvm::Value>, Error>
CodeGeneration::visit(const AST_VECTOR *node) const noexcept {
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

std::expected<std::shared_ptr<llvm::Value>, Error>
CodeGeneration::visit(const AST_INDEX *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_INDEX");
  }
  const auto result{node->index()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  return {};
}

} // namespace nicole
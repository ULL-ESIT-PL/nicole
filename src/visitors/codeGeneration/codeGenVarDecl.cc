#include "../../../inc/visitors/codeGeneration/codeGeneration.h"
#include "../../../inc/parsingAnalysis/ast/variables/ast_autoDecl.h"
#include "../../../inc/parsingAnalysis/ast/variables/ast_typedDecl.h"
#include "../../../inc/parsingAnalysis/ast/variables/ast_varCall.h"
#include <cstddef>
#include <memory>
#include <variant>

namespace nicole {

std::expected<std::shared_ptr<llvm::Value>, Error>
CodeGeneration::visit(const AST_AUTO_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_AUTO_DECL");
  }
  const auto result{node->value()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  return {};
}

std::expected<std::shared_ptr<llvm::Value>, Error>
CodeGeneration::visit(const AST_VAR_TYPED_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_VAR_TYPED_DECL");
  }
  const auto result{node->value()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  return {};
}

std::expected<std::shared_ptr<llvm::Value>, Error>
CodeGeneration::visit(const AST_VAR_CALL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_VAR_CALL");
  }
  return {};
}

}
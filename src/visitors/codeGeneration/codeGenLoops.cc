#include "../../../inc/visitors/codeGeneration/codeGeneration.h"

#include "../../../inc/parsingAnalysis/ast/loops/ast_doWhile.h"
#include "../../../inc/parsingAnalysis/ast/loops/ast_for.h"
#include "../../../inc/parsingAnalysis/ast/loops/ast_pass.h"
#include "../../../inc/parsingAnalysis/ast/loops/ast_stop.h"
#include "../../../inc/parsingAnalysis/ast/loops/ast_while.h"
#include <cstddef>
#include <memory>
#include <variant>

namespace nicole {

std::expected<std::shared_ptr<llvm::Value>, Error>
CodeGeneration::visit(const AST_WHILE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_WHILE");
  }
  const auto condition{node->condition()->accept(*this)};
  if (!condition) {
    return createError(condition.error());
  }
  const auto result{node->body()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  return {};
}

std::expected<std::shared_ptr<llvm::Value>, Error>
CodeGeneration::visit(const AST_FOR *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_FOR");
  }
  for (const auto &expr : node->init()) {
    const auto result{expr->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }
  const auto condition{node->condition()->accept(*this)};
  if (!condition) {
    return createError(condition.error());
  }
  for (const auto &expr : node->update()) {
    const auto result{expr->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }
  const auto result{node->body()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  return {};
}

std::expected<std::shared_ptr<llvm::Value>, Error>
CodeGeneration::visit(const AST_DO_WHILE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DO_WHILE");
  }
  const auto result{node->body()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  const auto condition{node->condition()->accept(*this)};
  if (!condition) {
    return createError(condition.error());
  }
  return {};
}

std::expected<std::shared_ptr<llvm::Value>, Error>
CodeGeneration::visit(const AST_PASS *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_PASS");
  }
  return {};
}

std::expected<std::shared_ptr<llvm::Value>, Error>
CodeGeneration::visit(const AST_STOP *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_STOP");
  }
  return {};
}

}
#include "../../../inc/parsingAnalysis/ast/userTypes/ast_attrAccess.h"
#include "../../../inc/parsingAnalysis/ast/userTypes/ast_constructorCall.h"
#include "../../../inc/parsingAnalysis/ast/userTypes/ast_methodCall.h"
#include "../../../inc/parsingAnalysis/ast/userTypes/ast_struct.h"
#include "../../../inc/parsingAnalysis/ast/userTypes/ast_this.h"
#include "../../../inc/visitors/codeGeneration/codeGeneration.h"

namespace nicole {

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_STRUCT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_STRUCT");
  }
  const std::expected<llvm::Value *, Error> constructor{
      node->constructor()->accept(*this)};
  if (!constructor) {
    return createError(constructor.error());
  }
  const std::expected<llvm::Value *, Error> destructor{
      node->destructor()->accept(*this)};
  if (!destructor) {
    return createError(destructor.error());
  }
  for (const std::shared_ptr<AST_METHOD_DECL> &chain : node->methods()) {
    const std::expected<llvm::Value *, Error> result{chain->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }
  return {};
}

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_ATTR_ACCESS *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ATTR_ACCESS");
  }
  return {};
}

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_METHOD_CALL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_METHOD_CALL");
  }
  for (const std::shared_ptr<AST> &chain : node->parameters()) {
    const std::expected<llvm::Value *, Error> result{chain->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }
  return {};
}

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_METHOD_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_METHOD_DECL");
  }
  const std::expected<llvm::Value *, Error> result{node->body()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  return {};
}

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_CONSTRUCTOR_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_CONSTRUCTOR_DECL");
  }
  if (node->super()) {
    const std::expected<llvm::Value *, Error> result{
        node->super()->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }
  const std::expected<llvm::Value *, Error> result{node->body()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  return {};
}

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_SUPER *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_SUPER");
  }
  for (const std::shared_ptr<AST> &chain : node->arguments()) {
    const std::expected<llvm::Value *, Error> result{chain->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }
  return {};
}

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_DESTRUCTOR_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_DESTRUCTOR_DECL");
  }
  const std::expected<llvm::Value *, Error> result{node->body()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  return {};
}

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_THIS *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_THIS");
  }
  return {};
}

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_CONSTRUCTOR_CALL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_CONSTRUCTOR_CALL");
  }
  for (const std::shared_ptr<AST> &chain : node->parameters()) {
    const std::expected<llvm::Value *, Error> result{chain->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }
  return {};
}

} // namespace nicole
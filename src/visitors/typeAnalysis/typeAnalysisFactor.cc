#include "../../../inc/visitors/typeAnalysis/typeAnalysis.h"
#include "../../../inc/parsingAnalysis/ast/literals/ast_bool.h"
#include "../../../inc/parsingAnalysis/ast/literals/ast_char.h"
#include "../../../inc/parsingAnalysis/ast/literals/ast_double.h"
#include "../../../inc/parsingAnalysis/ast/literals/ast_float.h"
#include "../../../inc/parsingAnalysis/ast/literals/ast_int.h"
#include "../../../inc/parsingAnalysis/ast/literals/ast_null.h"
#include "../../../inc/parsingAnalysis/ast/literals/ast_string.h"

namespace nicole {

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_BOOL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_BOOL");
  }
  return typeTable_->getType("bool");
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_CHAR *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_CHAR");
  }
  return typeTable_->getType("char");
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_DOUBLE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DOUBLE");
  }
  return typeTable_->getType("double");
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_FLOAT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_FLOAT");
  }
  return typeTable_->getType("float");
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_INT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_INT");
  }
  return typeTable_->getType("int");
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_NULL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_NULL");
  }
  return typeTable_->null();
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_STRING *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_STRING");
  }
  return typeTable_->getType("str");
}

}
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
  const auto type{*typeTable_->getType("bool")};
  node->setReturnedFromAnalysis(type);
  return type;
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_CHAR *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_CHAR");
  }
  const auto type{*typeTable_->getType("char")};
  node->setReturnedFromAnalysis(type);
  return type;
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_DOUBLE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DOUBLE");
  }
  const auto type{*typeTable_->getType("double")};
  node->setReturnedFromAnalysis(type);
  return type;
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_FLOAT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_FLOAT");
  }
  const auto type{*typeTable_->getType("float")};
  node->setReturnedFromAnalysis(type);
  return type;
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_INT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_INT");
  }
  const auto type{*typeTable_->getType("int")};
  node->setReturnedFromAnalysis(type);
  return type;
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_NULL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_NULL");
  }
  const auto type{typeTable_->null()};
  node->setReturnedFromAnalysis(type);
  return type;
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_STRING *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_STRING");
  }
  const auto type{*typeTable_->getType("str")};
  node->setReturnedFromAnalysis(type);
  return type;
}

}
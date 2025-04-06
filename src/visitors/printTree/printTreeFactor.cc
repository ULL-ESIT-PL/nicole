#include "../../../inc/visitors/printTree/printTree.h"
#include "../../../inc/parsingAnalysis/ast/literals/ast_bool.h"
#include "../../../inc/parsingAnalysis/ast/literals/ast_char.h"
#include "../../../inc/parsingAnalysis/ast/literals/ast_double.h"
#include "../../../inc/parsingAnalysis/ast/literals/ast_float.h"
#include "../../../inc/parsingAnalysis/ast/literals/ast_int.h"
#include "../../../inc/parsingAnalysis/ast/literals/ast_null.h"
#include "../../../inc/parsingAnalysis/ast/literals/ast_string.h"

namespace nicole {

std::expected<std::string, Error>
PrintTree::visit(const AST_BOOL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_BOOL");
  }
  return indent_ + "Bool: " + (node->value() ? "true" : "false") + "\n";
}

std::expected<std::string, Error>
PrintTree::visit(const AST_CHAR *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_CHAR");
  }
  return indent_ + "Char: '" + node->value() + "'\n";
}

std::expected<std::string, Error>
PrintTree::visit(const AST_DOUBLE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DOUBLE");
  }
  return indent_ + "Double: " + std::to_string(node->value()) + "\n";
}

std::expected<std::string, Error>
PrintTree::visit(const AST_FLOAT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_FLOAT");
  }
  return indent_ + "Float: " + std::to_string(node->value()) + "\n";
}

std::expected<std::string, Error>
PrintTree::visit(const AST_INT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_INT");
  }
  return indent_ + "Integer: " + std::to_string(node->value()) + "\n";
}

std::expected<std::string, Error>
PrintTree::visit(const AST_NULL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_NULL");
  }
  return indent_ + "null\n";
}

std::expected<std::string, Error>
PrintTree::visit(const AST_STRING *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_STRING");
  }
  return indent_ + "String: \"" + node->value() + "\"\n";
}

}
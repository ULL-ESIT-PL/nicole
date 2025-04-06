#include "../../../inc/visitors/printTree/printTree.h"
#include "../../../inc/parsingAnalysis/ast/operators/ast_binary.h"
#include "../../../inc/parsingAnalysis/ast/operators/ast_unary.h"
#include <ostream>

namespace nicole {

std::expected<std::string, Error>
PrintTree::visit(const AST_BINARY *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_BINARY");
  }
  std::ostringstream result;
  result << indent_ << node->op().raw() + ":\n";
  increaseIndent();
  const auto left{node->left()->accept(*this)};
  if (!left) {
    return createError(left.error());
  }
  result << indent_ << "Left:\n" << *left;
  const auto right{node->right()->accept(*this)};
  if (!right) {
    return createError(right.error());
  }
  result << indent_ << "Right:\n" << *right;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_UNARY *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_UNARY");
  }
  std::ostringstream result;
  result << indent_ << node->op().raw() + ":\n";
  increaseIndent();
  const auto val{node->value()->accept(*this)};
  if (!val) {
    return createError(val.error());
  }
  result << indent_ << "value:\n" << *val;
  decreaseIndent();
  return result.str();
}

}
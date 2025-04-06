#include "../../../inc/visitors/printTree/printTree.h"
#include "../../../inc/parsingAnalysis/ast/enum/ast_enum.h"
#include "../../../inc/parsingAnalysis/ast/enum/ast_enumAccess.h"
#include <ostream>

namespace nicole {

std::expected<std::string, Error>
PrintTree::visit(const AST_ENUM *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ENUM");
  }
  std::ostringstream result;
  result << indent_ << "Enum:\n";
  increaseIndent();
  result << indent_ << "id: " << node->id() << "\n";
  result << indent_ << "names:\n";
  for (const auto &id : node->identifiers()) {
    result << indent_ << id << "\n";
  }
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_ENUM_ACCESS *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ENUM_ACCESS");
  }
  std::ostringstream result;
  result << indent_ << "Enum Access:\n";
  increaseIndent();
  result << indent_ << "id: " << node->enumId() << "\n";
  result << indent_ << "name: " + node->identifier() + "\n";
  decreaseIndent();
  return result.str();
}

}
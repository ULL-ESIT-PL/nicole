#include "../../../inc/visitors/typeAnalysis/typeAnalysis.h"
#include "../../../inc/parsingAnalysis/ast/enum/ast_enum.h"
#include "../../../inc/parsingAnalysis/ast/enum/ast_enumAccess.h"
#include <memory>

namespace nicole {

/*
- retorna NoPropagate
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_ENUM *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ENUM");
  }
  const auto type{typeTable_->noPropagateType()};
  node->setReturnedFromAnalysis(type);
  return type;
}

/*
- retorna int
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_ENUM_ACCESS *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ENUM_ACCESS");
  }
  const auto type{typeTable_->getType(node->enumId())};
  if (!type) {
    return createError(ERROR_TYPE::TYPE, "enum does not exist");
  }
  node->setReturnedFromAnalysis(*type);
  return type;
}

}
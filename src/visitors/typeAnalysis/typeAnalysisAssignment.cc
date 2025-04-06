#include "../../../inc/visitors/typeAnalysis/typeAnalysis.h"
#include "../../../inc/parsingAnalysis/ast/assignments/ast_assignment.h"
#include <memory>

namespace nicole {

/*
- comprueba que left no sea void ni NoPropagate, BreakType
- comrpueba que right no sea void ni NoPropagate, BreakType
- comprueba que sea asignable
- retorna NoPropagate
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_ASSIGNMENT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ASSIGNMENT");
  }
  const auto left{node->left()->accept(*this)};
  if (!left) {
    return createError(left.error());
  }
  const auto right{node->value()->accept(*this)};
  if (!right) {
    return createError(right.error());
  }
  if (!typeTable_->canAssign(left.value(), right.value()))
    return createError(ERROR_TYPE::TYPE, "incompatible types in assignment");
  return typeTable_->noPropagateType();
}

}
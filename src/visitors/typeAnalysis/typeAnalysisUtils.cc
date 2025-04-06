#include "../../../inc/visitors/typeAnalysis/typeAnalysis.h"
#include "../../../inc/parsingAnalysis/ast/utils/ast_import.h"
#include "../../../inc/parsingAnalysis/ast/utils/ast_print.h"
#include <memory>

namespace nicole {

/*
- comprueba que los tipos de los argumentos no sean void ni NoPropagate,
BreakType
- si es un userType comprueba que exista el metodo toStr()
- retorna NoPropagate
 */
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_PRINT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_PRINT");
  }
  for (const auto &expr : node->values()) {
    const auto result{expr->accept(*this)};
    if (!result) {
      return createError(result.error());
    }

    auto exprType = result.value();

    if (insideDeclWithGenerics &&
        typeTable_->isGenericType(exprType, currentGenericList_))
      continue;

    if (auto userType = std::dynamic_pointer_cast<UserType>(exprType)) {
      auto methodsExp = userType->getMethods("toString");
      if (!methodsExp)
        return createError(methodsExp.error());
      if (methodsExp.value().empty())
        return createError(ERROR_TYPE::TYPE, "User type " + userType->name() +
                                                 " must implement toString()");
    }
  }
  return typeTable_->noPropagateType();
}

/*
- retorna NoPropagate
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_IMPORT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_IMPORT");
  }
  return typeTable_->noPropagateType();
}

}
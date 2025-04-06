#include "../../../inc/visitors/typeAnalysis/typeAnalysis.h"
#include "../../../inc/parsingAnalysis/ast/chained/ast_chained.h"
#include <memory>

namespace nicole {

/*
- comprueba que base no sea void
- retorna ultimo tipo encontrado
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_CHAINED *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_CHAINED");
  }
  auto baseRes = node->base()->accept(*this);
  if (!baseRes)
    return createError(baseRes.error());
  currentType_ = baseRes.value();

  for (const auto &op : node->operations()) {
    auto opRes = op->accept(*this);
    if (!opRes)
      return createError(opRes.error());
  }

  return currentType_;
}

}
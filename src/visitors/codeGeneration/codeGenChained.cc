#include "../../../inc/visitors/codeGeneration/codeGeneration.h"

#include "../../../inc/parsingAnalysis/ast/chained/ast_chained.h"

#include <memory>

namespace nicole {

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_CHAINED *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_CHAINED");
  }

  // Generar y guardar el valor base
  auto baseOrErr = node->base()->accept(*this);
  if (!baseOrErr)
    return createError(baseOrErr.error());
  // Guarda en el atributo
  resultChainedExpression_ = *baseOrErr;
  currentType = node->base()->returnedFromTypeAnalysis();

  // Para cada operación, invoca accept: cada visit actualizará
  // resultChainedExpression_
  for (const auto &op : node->operations()) {
    auto opOrErr = op->accept(*this);
    if (!opOrErr)
      return createError(opOrErr.error());
    // No necesitamos nada más aquí: resultChainedExpression_ ya apunta al nuevo
    // valor
    currentType = op->returnedFromTypeAnalysis();
  }

  // Devolver el valor acumulado
  return resultChainedExpression_;
}

} // namespace nicole
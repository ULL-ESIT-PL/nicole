#include "../../../inc/visitors/monomorphize/monomorphize.h"
#include "../../../inc/parsingAnalysis/ast/tree.h"
#include <variant>

/**

FillSemanticInfo ---> insertar delcaraciones en las tablas / insertar tipos /
comprobar que las variables pertenecen al scope (llamadas a variables) /
comrpobar llamadas a enum

TypeAnalysis ---> comprobar en una llamada a funcion que esta existe debido a
sobrecarga de funciones requiere que se trate en el typeAnalysis / igual con
metodos / llamadas a atributos / variables auto

 */

namespace nicole {

std::expected<std::monostate, Error>
Monomorphize::visit(const AST_STATEMENT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_STATEMENT");
  }
  return node->expression()->accept(*this);
}

std::expected<std::monostate, Error>
Monomorphize::visit(const AST_BODY *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_BODY");
  }
  for (const auto &statement : node->body()) {
    const auto val{statement->accept(*this)};
    if (!val) {
      return createError(val.error());
    }
  }
  return {};
}

std::expected<std::monostate, Error>
Monomorphize::visit(const Tree *tree) const noexcept {
  if (!tree) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid Tree");
  }
  return tree->root()->accept(*this);
}

} // namespace nicole
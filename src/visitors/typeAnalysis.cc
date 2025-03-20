#include "../../inc/visitors/typeAnalysis.h"
#include "../../inc/parsingAnalysis/ast/literals/ast_bool.h"
#include "../../inc/parsingAnalysis/ast/literals/ast_char.h"
#include "../../inc/parsingAnalysis/ast/literals/ast_double.h"
#include "../../inc/parsingAnalysis/ast/literals/ast_float.h"
#include "../../inc/parsingAnalysis/ast/literals/ast_int.h"
#include "../../inc/parsingAnalysis/ast/literals/ast_null.h"
#include "../../inc/parsingAnalysis/ast/literals/ast_string.h"

#include "../../inc/parsingAnalysis/ast/vector/ast_index.h"
#include "../../inc/parsingAnalysis/ast/vector/ast_vector.h"

#include "../../inc/parsingAnalysis/ast/pointer/ast_delete.h"
#include "../../inc/parsingAnalysis/ast/pointer/ast_deref.h"
#include "../../inc/parsingAnalysis/ast/pointer/ast_new.h"

#include "../../inc/parsingAnalysis/ast/operators/ast_binary.h"
#include "../../inc/parsingAnalysis/ast/operators/ast_unary.h"

#include "../../inc/parsingAnalysis/ast/assignments/ast_assignment.h"

#include "../../inc/parsingAnalysis/ast/utils/ast_import.h"
#include "../../inc/parsingAnalysis/ast/utils/ast_print.h"

#include "../../inc/parsingAnalysis/ast/loops/ast_doWhile.h"
#include "../../inc/parsingAnalysis/ast/loops/ast_for.h"
#include "../../inc/parsingAnalysis/ast/loops/ast_pass.h"
#include "../../inc/parsingAnalysis/ast/loops/ast_stop.h"
#include "../../inc/parsingAnalysis/ast/loops/ast_while.h"

#include "../../inc/parsingAnalysis/ast/conditionals/ast_case.h"
#include "../../inc/parsingAnalysis/ast/conditionals/ast_default.h"
#include "../../inc/parsingAnalysis/ast/conditionals/ast_elseIf.h"
#include "../../inc/parsingAnalysis/ast/conditionals/ast_if.h"
#include "../../inc/parsingAnalysis/ast/conditionals/ast_switch.h"
#include "../../inc/parsingAnalysis/ast/conditionals/ast_ternary.h"

#include "../../inc/parsingAnalysis/ast/functions/ast_funcCall.h"
#include "../../inc/parsingAnalysis/ast/functions/ast_funcDecl.h"
#include "../../inc/parsingAnalysis/ast/functions/ast_return.h"

#include "../../inc/parsingAnalysis/ast/enum/ast_enum.h"
#include "../../inc/parsingAnalysis/ast/enum/ast_enumAccess.h"
#include "../../inc/parsingAnalysis/ast/userTypes/ast_attrAccess.h"
#include "../../inc/parsingAnalysis/ast/userTypes/ast_constructorCall.h"
#include "../../inc/parsingAnalysis/ast/userTypes/ast_methodCall.h"
#include "../../inc/parsingAnalysis/ast/userTypes/ast_struct.h"
#include "../../inc/parsingAnalysis/ast/userTypes/ast_this.h"

#include "../../inc/parsingAnalysis/ast/variables/ast_autoDecl.h"
#include "../../inc/parsingAnalysis/ast/variables/ast_typedDecl.h"
#include "../../inc/parsingAnalysis/ast/variables/ast_varCall.h"

#include "../../inc/parsingAnalysis/ast/chained/ast_chained.h"

#include "../../inc/parsingAnalysis/ast/tree.h"
#include <cstddef>
#include <memory>
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

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_BOOL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_BOOL");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_CHAR *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_CHAR");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_DOUBLE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DOUBLE");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_FLOAT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_FLOAT");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_INT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_INT");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_NULL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_NULL");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_STRING *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_STRING");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_VECTOR *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_VECTOR");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_INDEX *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_INDEX");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_DELETE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DELETE");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_NEW *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_NEW");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_DEREF *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DEREF");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_BINARY *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_BINARY");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_UNARY *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_UNARY");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_ASSIGNMENT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ASSIGNMENT");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_PRINT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_PRINT");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_IMPORT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_IMPORT");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_STATEMENT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_STATEMENT");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_BODY *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_BODY");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_WHILE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_WHILE");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_FOR *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_FOR");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_DO_WHILE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DO_WHILE");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_PASS *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_PASS");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_STOP *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_STOP");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_IF *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_IF");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_ELSE_IF *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ELSE_IF");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_SWITCH *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_SWITCH");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_CASE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_CASE");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_DEFAULT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DEFAULT");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_TERNARY *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_TERNARY");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_CONDITION *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_CONDITION");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_FUNC_CALL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_FUNC_CALL");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_FUNC_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_FUNC_DECL");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_RETURN *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_RETURN");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_ENUM *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ENUM");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_ENUM_ACCESS *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ENUM_ACCESS");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_STRUCT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_STRUCT");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_ATTR_ACCESS *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ATTR_ACCESS");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_METHOD_CALL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_METHOD_CALL");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_METHOD_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_METHOD_DECL");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_CONSTRUCTOR_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_CONSTRUCTOR_DECL");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_SUPER *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_SUPER");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_DESTRUCTOR_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_DESTRUCTOR_DECL");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_THIS *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_THIS");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_CONSTRUCTOR_CALL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_CONSTRUCTOR_CALL");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_AUTO_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_AUTO_DECL");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_VAR_TYPED_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_VAR_TYPED_DECL");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_VAR_CALL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_VAR_CALL");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_CHAINED *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_CHAINED");
  }
  return {};
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const Tree *tree) const noexcept {
  if (!tree) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid Tree");
  }
  return {};
}

} // namespace nicole
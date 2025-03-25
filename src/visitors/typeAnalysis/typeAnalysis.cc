#include "../../../inc/visitors/typeAnalysis/typeAnalysis.h"
#include "../../../inc/parsingAnalysis/ast/literals/ast_bool.h"
#include "../../../inc/parsingAnalysis/ast/literals/ast_char.h"
#include "../../../inc/parsingAnalysis/ast/literals/ast_double.h"
#include "../../../inc/parsingAnalysis/ast/literals/ast_float.h"
#include "../../../inc/parsingAnalysis/ast/literals/ast_int.h"
#include "../../../inc/parsingAnalysis/ast/literals/ast_null.h"
#include "../../../inc/parsingAnalysis/ast/literals/ast_string.h"

#include "../../../inc/parsingAnalysis/ast/vector/ast_index.h"
#include "../../../inc/parsingAnalysis/ast/vector/ast_vector.h"

#include "../../../inc/parsingAnalysis/ast/pointer/ast_delete.h"
#include "../../../inc/parsingAnalysis/ast/pointer/ast_deref.h"
#include "../../../inc/parsingAnalysis/ast/pointer/ast_new.h"

#include "../../../inc/parsingAnalysis/ast/operators/ast_binary.h"
#include "../../../inc/parsingAnalysis/ast/operators/ast_unary.h"

#include "../../../inc/parsingAnalysis/ast/assignments/ast_assignment.h"

#include "../../../inc/parsingAnalysis/ast/utils/ast_import.h"
#include "../../../inc/parsingAnalysis/ast/utils/ast_print.h"

#include "../../../inc/parsingAnalysis/ast/loops/ast_doWhile.h"
#include "../../../inc/parsingAnalysis/ast/loops/ast_for.h"
#include "../../../inc/parsingAnalysis/ast/loops/ast_pass.h"
#include "../../../inc/parsingAnalysis/ast/loops/ast_stop.h"
#include "../../../inc/parsingAnalysis/ast/loops/ast_while.h"

#include "../../../inc/parsingAnalysis/ast/conditionals/ast_case.h"
#include "../../../inc/parsingAnalysis/ast/conditionals/ast_default.h"
#include "../../../inc/parsingAnalysis/ast/conditionals/ast_elseIf.h"
#include "../../../inc/parsingAnalysis/ast/conditionals/ast_if.h"
#include "../../../inc/parsingAnalysis/ast/conditionals/ast_switch.h"
#include "../../../inc/parsingAnalysis/ast/conditionals/ast_ternary.h"

#include "../../../inc/parsingAnalysis/ast/functions/ast_funcCall.h"
#include "../../../inc/parsingAnalysis/ast/functions/ast_funcDecl.h"
#include "../../../inc/parsingAnalysis/ast/functions/ast_return.h"

#include "../../../inc/parsingAnalysis/ast/enum/ast_enum.h"
#include "../../../inc/parsingAnalysis/ast/enum/ast_enumAccess.h"
#include "../../../inc/parsingAnalysis/ast/userTypes/ast_attrAccess.h"
#include "../../../inc/parsingAnalysis/ast/userTypes/ast_constructorCall.h"
#include "../../../inc/parsingAnalysis/ast/userTypes/ast_methodCall.h"
#include "../../../inc/parsingAnalysis/ast/userTypes/ast_struct.h"
#include "../../../inc/parsingAnalysis/ast/userTypes/ast_this.h"

#include "../../../inc/parsingAnalysis/ast/variables/ast_autoDecl.h"
#include "../../../inc/parsingAnalysis/ast/variables/ast_typedDecl.h"
#include "../../../inc/parsingAnalysis/ast/variables/ast_varCall.h"

#include "../../../inc/parsingAnalysis/ast/chained/ast_chained.h"

#include "../../../inc/parsingAnalysis/ast/tree.h"
#include <cstddef>
#include <iostream>
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
  return typeTable_->getType("bool");
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_CHAR *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_CHAR");
  }
  return typeTable_->getType("char");
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_DOUBLE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DOUBLE");
  }
  return typeTable_->getType("double");
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_FLOAT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_FLOAT");
  }
  return typeTable_->getType("float");
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_INT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_INT");
  }
  return typeTable_->getType("int");
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_NULL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_NULL");
  }
  return typeTable_->null();
}

std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_STRING *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_STRING");
  }
  return typeTable_->getType("str");
}

/*
- si esta vacio retorna un vector<null>
- si no esta vacio, comprueba que todos los elementos sean de un mismo tipo
- si un elemento es un puntero userType debe comprobar que todos parten de una
misma base
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_VECTOR *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_VECTOR");
  }
  return {};
}

/*
- el value debe ser de tipo int
- se apoya en currentType para deducir el tipo del elemento al que accede
 */
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_INDEX *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_INDEX");
  }
  return {};
}

/*
- value debe ser un puntero y que este en la tabla de memoria dinamica
- retorna NoPropagate
 */
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_DELETE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DELETE");
  }
  return {};
}

/*
- envuelve en un puntero la expression y lo retorna
 */
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_NEW *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_NEW");
  }
  return {};
}

/*
- la expresion debe ser un puntero y retorna el tipo al que apunta
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_DEREF *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DEREF");
  }
  return {};
}

/*
- comrpueba que left no sea void ni NoPropagate, BreakType
- comrpueba que right no sea void ni NoPropagate, BreakType
- comrprueba que se puedan operar, caso speciales si un userType busca metodo
para sobrecarga de operador
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_BINARY *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_BINARY");
  }
  return {};
}

/*
- comprueba que expression no es void ni NoPropagate, BreakType
- comrpueba que se puede operar, caso speciales si un userType busca metodo
para sobrecarga de operador y retorna el tipo
 */
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_UNARY *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_UNARY");
  }
  return {};
}

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
  return {};
}

/*
- comprueba que los tipos de los argumentos no sean void ni NoPropagate, BreakType
- si es un userType comprueba que exista el metodo toStr()
- retorna NoPropagate
 */
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_PRINT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_PRINT");
  }
  return {};
}

/*
- retorna NoPropagate
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_IMPORT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_IMPORT");
  }
  return {};
}

/*
- retorna el tipo de la expresion
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_STATEMENT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_STATEMENT");
  }
  return node->expression()->accept(*this);
}

/*
- si encuentra pass y stop devuelve void
- si encuentra un return retorna su tipo, vijilar varios returns en diferentes
body anidados
- si no encuentra nada retorna NoPropagate
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_BODY *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_BODY");
  }
  return {};
}

/*
- condtion debe retornar bool
- si body encuentra algo diferente a NoPropagate lo propaga si no retorna
NoPropagate
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_WHILE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_WHILE");
  }
  return {};
}

/*
- init ???
- condtion reotrna bool
- update ???
- body lo mismo que los otros
 */
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_FOR *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_FOR");
  }
  return {};
}

/*
- si body encuentra algo diferente a NoPropagate lo propaga si no retorna
NoPropagate
- condtion debe retornar bool
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_DO_WHILE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DO_WHILE");
  }
  return {};
}

/*
- retorna BreakType
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_PASS *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_PASS");
  }
  return {};
}

/*
- retorna BreakType
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_STOP *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_STOP");
  }
  return {};
}

/*
- codition debe ser bool
- body lo mismo que el resto
- else if lo mismo que if
- else lo mismo que body
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_IF *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_IF");
  }
  return {};
}

/*
- lo mismo que if
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_ELSE_IF *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ELSE_IF");
  }
  return {};
}

/*
- match solo bool, int, char
- Lo mismo que if
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_SWITCH *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_SWITCH");
  }
  return {};
}

/*
- match solo bool, int char
- igual que body
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_CASE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_CASE");
  }
  return {};
}

/*
- igual que body
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_DEFAULT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DEFAULT");
  }
  return {};
}

/*
- condtion debe ser bool
- first y second no pueden ser breakType
 */
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_TERNARY *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_TERNARY");
  }
  return {};
}

/*
- debe retornar bool
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_CONDITION *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_CONDITION");
  }
  return typeTable_->getType("bool");
}

/*
- numero correcto de generics en caso de tener
- parametros son del tipo correcto (no pueden ser void ni punteros a este)
- body debe retornar un tipo compatible al tipo de la funcion
- si no tiene generics debe existir una función con esos argumentos
- retorna tipo de función
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_FUNC_CALL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_FUNC_CALL");
  }
  return {};
}

/*
- parametros no pueden ser void ni punteros a este
- comprobar si algo es generic
- body debe retornar un tipo compatible al tipo de la funcion
- retorna el tipo NoPropagate
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_FUNC_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_FUNC_DECL");
  }
  return {};
}

/*
- si esta vacio retorna void
- si no el tipo de la expresion
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_RETURN *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_RETURN");
  }
  return {};
}

/*
- retorna NoPropagate
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_ENUM *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ENUM");
  }
  return {};
}

/*
- retorna int
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_ENUM_ACCESS *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ENUM_ACCESS");
  }
  return typeTable_->getType("int");
}

/*
- comprobar tipo atributos no sea void ni puntero a est, comprobar si es generic
- comrpobar metodos
- comproabr constructor
- comprobar destructor
- retorna NoPropagate
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_STRUCT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_STRUCT");
  }
  return {};
}

/*
- comprobar que sea genric
- se apoya en currentType para saber si el atributo existe y retorna el tipo
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_ATTR_ACCESS *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ATTR_ACCESS");
  }
  return {};
}

/*
- tener en cuenta generics
- se apoya en currentType para saber si el metodo existe y retorna el tipo
- igual que funcion
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_METHOD_CALL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_METHOD_CALL");
  }
  return {};
}

/*
- igual que funcion
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_METHOD_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_METHOD_DECL");
  }
  return {};
}

/*
- igual que funcion
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_CONSTRUCTOR_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_CONSTRUCTOR_DECL");
  }
  return {};
}

/*
- retorna el tipo del padre
- comprobar generics y argumentos
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_SUPER *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_SUPER");
  }
  return typeTable_->getType(node->fatherType()->toString());
}

/*
- comprobar body
- retorna NoPropagate
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_DESTRUCTOR_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_DESTRUCTOR_DECL");
  }
  return {};
}

/*
- retorna el tipo de la clase envuelto en un puntero const?
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_THIS *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_THIS");
  }
  return node->userType();
}

/*
- comprueba generics
- comprueba argumentos
- comprueba body
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_CONSTRUCTOR_CALL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_CONSTRUCTOR_CALL");
  }
  return typeTable_->getType(node->id());
}

/*
- comrpueba que el tipo de la expresion no sea void
- fija en currentScope el tipo
- retorna NoPropagate
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_AUTO_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_AUTO_DECL");
  }
  return {};
}

/*
- comrpueba que el tipo de la expresion no sea body
- comrpueba que la expresion sea compatible
- retorna NoPropagate
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_VAR_TYPED_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_VAR_TYPED_DECL");
  }
  return {};
}

/*
- se apoya en currentScope
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_VAR_CALL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_VAR_CALL");
  }
  return {};
}

/*
- comprueba que base no sea void
- retorna ultimo tipo encontrado
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_CHAINED *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_CHAINED");
  }
  return {};
}

/*
- comprueba que no hubo errores
- retorna int / void
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const Tree *tree) const noexcept {
  if (!tree) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid Tree");
  }
  return {};
}

} // namespace nicole
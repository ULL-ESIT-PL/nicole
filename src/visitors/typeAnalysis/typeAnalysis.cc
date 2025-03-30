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
  for (const auto &expr : node->values()) {
    const auto result{expr->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
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
  const auto result{node->index()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  if (analyzingInsideClass and
      typeTable_->isGenericType(*result, currentGenericList_)) {
    returnedGeneric = true;
    return nullptr;
  }
  if (!typeTable_->areSameType(*result, *typeTable_->getType("int"))) {
    return createError(ERROR_TYPE::TYPE, "index must be type int");
  }
  const auto vectorType{std::dynamic_pointer_cast<VectorType>(currentType_)};
  if (!vectorType) {
    return createError(ERROR_TYPE::TYPE, "can only access to vectors");
  }
  return vectorType->elementType();
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
  const auto result{node->value()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  const auto ptrType{std::dynamic_pointer_cast<PointerType>(currentType_)};
  if (!ptrType) {
    return createError(ERROR_TYPE::TYPE,
                       "can only delete the memory stored in a pointer");
  }
  return typeTable_->noPropagateType();
}

/*
- envuelve en un puntero la expression y lo retorna, debe ser constructor o
primitiva
 */
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_NEW *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_NEW");
  }
  const auto result{node->value()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  const auto basicType{std::dynamic_pointer_cast<BasicType>(*result)};
  const auto userType{std::dynamic_pointer_cast<UserType>(*result)};
  if (!basicType or !userType) {
    return createError(ERROR_TYPE::TYPE,
                       "can only use new with primtiives or user types");
  }
  return std::make_shared<PointerType>(*result);
}

/*
- la expresion debe ser un puntero y retorna el tipo al que apunta
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_DEREF *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DEREF");
  }
  const auto result{node->value()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  const auto ptrType{std::dynamic_pointer_cast<PointerType>(*result)};
  if (!ptrType) {
    return createError(ERROR_TYPE::TYPE, "can only deref a pointer");
  }
  return ptrType->baseType();
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
  const auto left{node->left()->accept(*this)};
  if (!left) {
    return createError(left.error());
  }
  const auto right{node->right()->accept(*this)};
  if (!right) {
    return createError(right.error());
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
  const auto value{node->value()->accept(*this)};
  if (!value) {
    return createError(value.error());
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
  const auto left{node->left()->accept(*this)};
  if (!left) {
    return createError(left.error());
  }
  const auto right{node->value()->accept(*this)};
  if (!right) {
    return createError(right.error());
  }
  return typeTable_->noPropagateType();
}

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
  currentScope_ = node->scope();
  for (const auto &expr : node->body()) {
    const auto result{expr->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }
  return typeTable_->noPropagateType();
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
  const auto condition{node->condition()->accept(*this)};
  if (!condition) {
    return createError(condition.error());
  }
  const auto body{node->body()->accept(*this)};
  if (!body) {
    return createError(body.error());
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
  for (const auto &expr : node->init()) {
    const auto result{expr->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }
  const auto condition{node->condition()->accept(*this)};
  if (!condition) {
    return createError(condition.error());
  }
  for (const auto &expr : node->update()) {
    const auto result{expr->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }
  const auto body{node->body()->accept(*this)};
  if (!body) {
    return createError(body.error());
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
  const auto body{node->body()->accept(*this)};
  if (!body) {
    return createError(body.error());
  }
  const auto condition{node->condition()->accept(*this)};
  if (!condition) {
    return createError(condition.error());
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
  return typeTable_->breakType();
}

/*
- retorna BreakType
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_STOP *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_STOP");
  }
  return typeTable_->breakType();
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
  const auto condition{node->condition()->accept(*this)};
  if (!condition) {
    return createError(condition.error());
  }
  const auto body{node->body()->accept(*this)};
  if (!body) {
    return createError(body.error());
  }
  for (const auto &elseIf : node->elseIf()) {
    const auto result{elseIf->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }
  if (node->elseBody()) {
    const auto elseBody{node->elseBody()->accept(*this)};
    if (!elseBody) {
      return createError(elseBody.error());
    }
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
  const auto condition{node->condition()->accept(*this)};
  if (!condition) {
    return createError(condition.error());
  }
  const auto body{node->body()->accept(*this)};
  if (!body) {
    return createError(body.error());
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
  const auto condition{node->condition()->accept(*this)};
  if (!condition) {
    return createError(condition.error());
  }
  for (const auto &case_ : node->cases()) {
    const auto result{case_->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }
  if (node->defaultCase()) {
    const auto defaultCase{node->defaultCase()->accept(*this)};
    if (!defaultCase) {
      return createError(defaultCase.error());
    }
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
  const auto match{node->match()->accept(*this)};
  if (!match) {
    return createError(match.error());
  }
  const auto body{node->body()->accept(*this)};
  if (!body) {
    return createError(body.error());
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
  const auto body{node->body()->accept(*this)};
  if (!body) {
    return createError(body.error());
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
  const auto condition{node->condition()->accept(*this)};
  if (!condition) {
    return createError(condition.error());
  }
  const auto first{node->first()->accept(*this)};
  if (!first) {
    return createError(first.error());
  }
  const auto second{node->second()->accept(*this)};
  if (!second) {
    return createError(second.error());
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
  const auto condition{node->condition()->accept(*this)};
  if (!condition) {
    return createError(condition.error());
  }
  return {};
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
  for (const auto &expr : node->parameters()) {
    const auto resul{expr->accept(*this)};
    if (!resul) {
      return createError(resul.error());
    }
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
  const auto body{node->body()->accept(*this)};
  if (!body) {
    return createError(body.error());
  }
  return typeTable_->noPropagateType();
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
  if (!node->expression()) {
    return *typeTable_->getType("void");
  }
  return node->expression()->accept(*this);
}

/*
- retorna NoPropagate
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_ENUM *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ENUM");
  }
  return typeTable_->noPropagateType();
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
  analyzingInsideClass = true;

  analyzingInsideClass = false;
  return typeTable_->noPropagateType();
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
  for (const auto &expr : node->parameters()) {
    const auto resul{expr->accept(*this)};
    if (!resul) {
      return createError(resul.error());
    }
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
  const auto body{node->body()->accept(*this)};
  if (!body) {
    return createError(body.error());
  }
  return typeTable_->noPropagateType();
}

/*
- igual que funcion
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_CONSTRUCTOR_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_CONSTRUCTOR_DECL");
  }
  const auto body{node->body()->accept(*this)};
  if (!body) {
    return createError(body.error());
  }
  return typeTable_->noPropagateType();
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
  for (const auto &arg : node->arguments()) {
    const auto result{arg->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
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
  const auto body{node->body()->accept(*this)};
  if (!body) {
    return createError(body.error());
  }
  return typeTable_->noPropagateType();
}

/*
- retorna el tipo de la clase envuelto en un puntero const?
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_THIS *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_THIS");
  }
  return {};
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
  for (const auto &expr : node->parameters()) {
    const auto resul{expr->accept(*this)};
    if (!resul) {
      return createError(resul.error());
    }
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
  const auto value{node->value()->accept(*this)};
  if (!value) {
    return createError(value.error());
  }
  return typeTable_->noPropagateType();
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
  const auto value{node->value()->accept(*this)};
  if (!value) {
    return createError(value.error());
  }
  return typeTable_->noPropagateType();
}

/*
- se apoya en currentScope y en en currenStruct para saber si es un atributo
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
  const auto base{node->base()->accept(*this)};
  if (!base) {
    return createError(base.error());
  }
  for (const auto &operations : node->operations()) {
    const auto result{operations->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
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
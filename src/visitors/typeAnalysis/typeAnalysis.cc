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
  const auto &values = node->values();
  if (values.empty())
    return std::make_shared<VectorType>(typeTable_->null());

  auto firstResult = values[0]->accept(*this);
  if (!firstResult)
    return createError(firstResult.error());
  auto commonType = firstResult.value();

  if (insideDeclWithGenerics &&
      typeTable_->isGenericType(commonType, currentGenericList_))
    commonType = std::make_shared<PlaceHolder>(commonType);

  for (size_t i = 1; i < values.size(); ++i) {
    auto result = values[i]->accept(*this);
    if (!result)
      return createError(result.error());
    auto elemType = result.value();

    if (auto constElem = std::dynamic_pointer_cast<ConstType>(elemType))
      elemType = constElem->baseType();

    if (insideDeclWithGenerics &&
        typeTable_->isGenericType(elemType, currentGenericList_))
      elemType = std::make_shared<PlaceHolder>(elemType);

    if (!typeTable_->areSameType(commonType, elemType)) {
      if (!typeTable_->haveCommonAncestor(commonType, elemType))
        return createError(ERROR_TYPE::TYPE,
                           "vector elements do not have a common type");
    }
  }

  return std::make_shared<VectorType>(commonType);
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
  if (insideDeclWithGenerics and
      typeTable_->isGenericType(*result, currentGenericList_)) {
    return std::make_shared<PlaceHolder>(*result);
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

  auto result = node->value()->accept(*this);
  if (!result) {
    return createError(result.error());
  }

  auto type = result.value();

  if (insideDeclWithGenerics) {
    if (auto placeholder = std::dynamic_pointer_cast<PlaceHolder>(type)) {
      auto genericCompound = placeholder->getGenericCompound();
      if (std::dynamic_pointer_cast<PointerType>(genericCompound)) {
        return placeholder;
      }
      return createError(ERROR_TYPE::TYPE, "can only delete pointers");
    }
  }

  if (auto constType = std::dynamic_pointer_cast<ConstType>(type)) {
    if (std::dynamic_pointer_cast<PointerType>(constType->baseType())) {
      return type;
    }
    return createError(ERROR_TYPE::TYPE, "can only delete pointers");
  }

  if (std::dynamic_pointer_cast<PointerType>(type)) {
    return typeTable_->noPropagateType();
  }

  return createError(ERROR_TYPE::TYPE, "can only delete pointers");
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

  auto result = node->value()->accept(*this);
  if (!result)
    return createError(result.error());

  auto exprType = result.value();

  if (insideDeclWithGenerics and
      typeTable_->isGenericType(exprType, currentGenericList_))
    return std::make_shared<PointerType>(exprType);

  auto basicType = std::dynamic_pointer_cast<BasicType>(exprType);
  auto userType = std::dynamic_pointer_cast<UserType>(exprType);
  if (!basicType && !userType)
    return createError(
        ERROR_TYPE::TYPE,
        "can only use new with primitives, user types or generics");

  return std::make_shared<PointerType>(exprType);
}

/*
- la expresion debe ser un puntero y retorna el tipo al que apunta
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_DEREF *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DEREF");
  }

  auto result = node->value()->accept(*this);
  if (!result)
    return createError(result.error());

  auto type = result.value();

  std::shared_ptr<Type> unwrappedType = type;
  if (auto constType = std::dynamic_pointer_cast<ConstType>(type))
    unwrappedType = constType->baseType();

  if (insideDeclWithGenerics &&
      typeTable_->isGenericType(unwrappedType, currentGenericList_)) {
    if (auto ptrType = std::dynamic_pointer_cast<PointerType>(unwrappedType))
      return std::make_shared<PlaceHolder>(ptrType->baseType());
    return createError(ERROR_TYPE::TYPE, "can only deref a pointer");
  }

  if (auto ptrType = std::dynamic_pointer_cast<PointerType>(unwrappedType))
    return ptrType->baseType();

  return createError(ERROR_TYPE::TYPE, "can only deref a pointer");
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
  if (!typeTable_->canAssign(left.value(), right.value()))
    return createError(ERROR_TYPE::TYPE, "incompatible types in assignment");
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
  std::vector<std::shared_ptr<Type>> returnTypes;
  bool foundBreak = false;

  for (const auto &expr : node->body()) {
    auto result = expr->accept(*this);
    if (!result)
      return createError(result.error());

    auto type = result.value();
    if (typeTable_->areSameType(type, typeTable_->noPropagateType()))
      continue;

    if (typeTable_->areSameType(type, typeTable_->breakType())) {
      foundBreak = true;
      continue;
    }

    returnTypes.push_back(type);
  }

  if (!returnTypes.empty() && foundBreak)
    return createError(ERROR_TYPE::TYPE,
                       "inconsistent return types: cannot mix return and "
                       "break/continue in the same body");

  if (!returnTypes.empty()) {
    auto commonType = returnTypes.front();
    for (size_t i = 1; i < returnTypes.size(); ++i) {
      if (!typeTable_->areSameType(commonType, returnTypes[i])) {
        if (!typeTable_->haveCommonAncestor(commonType, returnTypes[i])) {
          return createError(ERROR_TYPE::TYPE,
                             "inconsistent return types in body");
        }
        // se podría definir commonType como el ancestro común ????????
      }
    }
    return commonType;
  }
  if (foundBreak)
    return typeTable_->breakType();

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
  auto condition = node->condition()->accept(*this);
  if (!condition)
    return createError(condition.error());
  auto condType = condition.value();

  if (auto constCond = std::dynamic_pointer_cast<ConstType>(condType))
    condType = constCond->baseType();

  if (insideDeclWithGenerics &&
      typeTable_->isGenericType(condType, currentGenericList_))
    return std::make_shared<PlaceHolder>(condType);

  auto boolType = typeTable_->getType("bool");
  if (!typeTable_->areSameType(condType, *boolType))
    return createError(ERROR_TYPE::TYPE, "a condition must be boolean");

  auto body = node->body()->accept(*this);
  if (!body)
    return createError(body.error());
  auto bodyType = body.value();

  if (!typeTable_->areSameType(bodyType, typeTable_->noPropagateType()) &&
      !typeTable_->areSameType(bodyType, typeTable_->breakType())) {
    return bodyType;
  }

  return typeTable_->noPropagateType();
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
  if (!node->init().empty()) {
    for (const auto &expr : node->init()) {
      auto initResult = expr->accept(*this);
      if (!initResult)
        return createError(initResult.error());
    }
  }

  auto condition = node->condition()->accept(*this);
  if (!condition)
    return createError(condition.error());
  auto condType = condition.value();

  if (auto constCond = std::dynamic_pointer_cast<ConstType>(condType))
    condType = constCond->baseType();

  if (insideDeclWithGenerics &&
      typeTable_->isGenericType(condType, currentGenericList_))
    return std::make_shared<PlaceHolder>(condType);

  auto boolType = typeTable_->getType("bool");
  if (!typeTable_->areSameType(condType, *boolType))
    return createError(ERROR_TYPE::TYPE, "a condition must be boolean");

  if (!node->update().empty()) {
    for (const auto &expr : node->update()) {
      auto updateResult = expr->accept(*this);
      if (!updateResult)
        return createError(updateResult.error());
    }
  }

  auto body = node->body()->accept(*this);
  if (!body)
    return createError(body.error());
  auto bodyType = body.value();

  if (!typeTable_->areSameType(bodyType, typeTable_->noPropagateType()) &&
      !typeTable_->areSameType(bodyType, typeTable_->breakType())) {
    return bodyType;
  }

  return typeTable_->noPropagateType();
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
  auto body = node->body()->accept(*this);
  if (!body)
    return createError(body.error());
  auto bodyType = body.value();

  auto condition = node->condition()->accept(*this);
  if (!condition)
    return createError(condition.error());
  auto condType = condition.value();

  if (auto constCond = std::dynamic_pointer_cast<ConstType>(condType))
    condType = constCond->baseType();

  if (insideDeclWithGenerics &&
      typeTable_->isGenericType(condType, currentGenericList_))
    return std::make_shared<PlaceHolder>(condType);

  auto boolType = typeTable_->getType("bool");
  if (!typeTable_->areSameType(condType, *boolType))
    return createError(ERROR_TYPE::TYPE, "a condition must be boolean");

  if (!typeTable_->areSameType(bodyType, typeTable_->noPropagateType()) &&
      !typeTable_->areSameType(bodyType, typeTable_->breakType())) {
    return bodyType;
  }

  return typeTable_->noPropagateType();
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
  auto condition = node->condition()->accept(*this);
  if (!condition)
    return createError(condition.error());
  auto condType = condition.value();

  if (auto constCond = std::dynamic_pointer_cast<ConstType>(condType))
    condType = constCond->baseType();

  if (insideDeclWithGenerics &&
      typeTable_->isGenericType(condType, currentGenericList_))
    return std::make_shared<PlaceHolder>(condType);

  auto boolType = typeTable_->getType("bool");
  if (!typeTable_->areSameType(condType, *boolType))
    return createError(ERROR_TYPE::TYPE, "a condition must be boolean");

  auto mainBody = node->body()->accept(*this);
  if (!mainBody)
    return createError(mainBody.error());

  std::vector<std::shared_ptr<Type>> branchTypes;

  if (!typeTable_->areSameType(mainBody.value(),
                               typeTable_->noPropagateType()) &&
      !typeTable_->areSameType(mainBody.value(), typeTable_->breakType())) {
    branchTypes.push_back(mainBody.value());
  }

  for (const auto &elseIf : node->elseIf()) {
    auto elseIfResult = elseIf->accept(*this);
    if (!elseIfResult)
      return createError(elseIfResult.error());
    if (!typeTable_->areSameType(elseIfResult.value(),
                                 typeTable_->noPropagateType()) &&
        !typeTable_->areSameType(elseIfResult.value(),
                                 typeTable_->breakType())) {
      branchTypes.push_back(elseIfResult.value());
    }
  }

  if (node->elseBody()) {
    auto elseBody = node->elseBody()->accept(*this);
    if (!elseBody)
      return createError(elseBody.error());
    if (!typeTable_->areSameType(elseBody.value(),
                                 typeTable_->noPropagateType()) &&
        !typeTable_->areSameType(elseBody.value(), typeTable_->breakType())) {
      branchTypes.push_back(elseBody.value());
    }
  }

  if (!branchTypes.empty()) {
    auto commonType = branchTypes.front();
    for (size_t i = 1; i < branchTypes.size(); ++i) {
      if (!typeTable_->areSameType(commonType, branchTypes[i])) {
        if (!typeTable_->haveCommonAncestor(commonType, branchTypes[i])) {
          return createError(ERROR_TYPE::TYPE,
                             "inconsistent return types in if branches");
        }
      }
    }
    return commonType;
  }

  return typeTable_->noPropagateType();
}

/*
- lo mismo que if
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_ELSE_IF *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ELSE_IF");
  }
  auto condition = node->condition()->accept(*this);
  if (!condition)
    return createError(condition.error());
  auto condType = condition.value();

  if (auto constCond = std::dynamic_pointer_cast<ConstType>(condType))
    condType = constCond->baseType();

  if (insideDeclWithGenerics &&
      typeTable_->isGenericType(condType, currentGenericList_))
    return std::make_shared<PlaceHolder>(condType);

  auto boolType = typeTable_->getType("bool");
  if (!typeTable_->areSameType(condType, *boolType))
    return createError(ERROR_TYPE::TYPE, "a condition must be boolean");

  auto body = node->body()->accept(*this);
  if (!body)
    return createError(body.error());
  auto bodyType = body.value();

  if (!typeTable_->areSameType(bodyType, typeTable_->noPropagateType()) &&
      !typeTable_->areSameType(bodyType, typeTable_->breakType())) {
    return bodyType;
  }
  return typeTable_->noPropagateType();
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
  auto condition = node->condition()->accept(*this);
  if (!condition)
    return createError(condition.error());
  auto condType = condition.value();
  if (auto constCond = std::dynamic_pointer_cast<ConstType>(condType))
    condType = constCond->baseType();
  if (insideDeclWithGenerics &&
      typeTable_->isGenericType(condType, currentGenericList_))
    return std::make_shared<PlaceHolder>(condType);
  auto boolType = typeTable_->getType("bool");
  auto intType = typeTable_->getType("int");
  auto charType = typeTable_->getType("char");
  bool isEnum = (std::dynamic_pointer_cast<EnumType>(condType) != nullptr);
  if (!(typeTable_->areSameType(condType, *boolType) ||
        typeTable_->areSameType(condType, *intType) ||
        typeTable_->areSameType(condType, *charType) || isEnum)) {
    return createError(ERROR_TYPE::TYPE,
                       "switch condition must be bool, int, char, or enum");
  }
  switchTypeCondition_ = condType;
  std::vector<std::shared_ptr<Type>> branchTypes;
  for (const auto &caseNode : node->cases()) {
    auto caseResult = caseNode->accept(*this);
    if (!caseResult)
      return createError(caseResult.error());
    auto caseType = caseResult.value();
    if (!typeTable_->areSameType(caseType, typeTable_->noPropagateType()) &&
        !typeTable_->areSameType(caseType, typeTable_->breakType())) {
      branchTypes.push_back(caseType);
    }
  }
  if (node->defaultCase()) {
    auto defaultResult = node->defaultCase()->accept(*this);
    if (!defaultResult)
      return createError(defaultResult.error());
    auto defaultType = defaultResult.value();
    if (!typeTable_->areSameType(defaultType, typeTable_->noPropagateType()) &&
        !typeTable_->areSameType(defaultType, typeTable_->breakType())) {
      branchTypes.push_back(defaultType);
    }
  }
  if (!branchTypes.empty()) {
    auto commonType = branchTypes.front();
    for (size_t i = 1; i < branchTypes.size(); ++i) {
      if (!typeTable_->areSameType(commonType, branchTypes[i])) {
        if (!typeTable_->haveCommonAncestor(commonType, branchTypes[i])) {
          return createError(ERROR_TYPE::TYPE,
                             "inconsistent return types in switch branches");
        }
      }
    }
    return commonType;
  }
  return typeTable_->noPropagateType();
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
  auto matchResult = node->match()->accept(*this);
  if (!matchResult)
    return createError(matchResult.error());
  auto matchType = matchResult.value();

  if (auto constMatch = std::dynamic_pointer_cast<ConstType>(matchType))
    matchType = constMatch->baseType();

  if (insideDeclWithGenerics &&
      typeTable_->isGenericType(matchType, currentGenericList_))
    return std::make_shared<PlaceHolder>(matchType);

  if (!typeTable_->areSameType(matchType, switchTypeCondition_))
    return createError(ERROR_TYPE::TYPE,
                       "case match type does not match switch condition type");

  auto bodyResult = node->body()->accept(*this);
  if (!bodyResult)
    return createError(bodyResult.error());
  auto bodyType = bodyResult.value();

  if (!typeTable_->areSameType(bodyType, typeTable_->noPropagateType()) &&
      !typeTable_->areSameType(bodyType, typeTable_->breakType())) {
    return bodyType;
  }

  return typeTable_->noPropagateType();
}

/*
- igual que body
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_DEFAULT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DEFAULT");
  }
  auto bodyResult = node->body()->accept(*this);
  if (!bodyResult)
    return createError(bodyResult.error());
  auto bodyType = bodyResult.value();

  if (!typeTable_->areSameType(bodyType, typeTable_->noPropagateType()) &&
      !typeTable_->areSameType(bodyType, typeTable_->breakType())) {
    return bodyType;
  }
  return typeTable_->noPropagateType();
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
  auto condition = node->condition()->accept(*this);
  if (!condition)
    return createError(condition.error());
  auto condType = condition.value();

  if (auto constCond = std::dynamic_pointer_cast<ConstType>(condType))
    condType = constCond->baseType();

  if (insideDeclWithGenerics &&
      typeTable_->isGenericType(condType, currentGenericList_))
    return std::make_shared<PlaceHolder>(condType);

  auto boolType = typeTable_->getType("bool");
  if (!typeTable_->areSameType(condType, *boolType))
    return createError(ERROR_TYPE::TYPE, "a condition must be boolean");

  auto firstResult = node->first()->accept(*this);
  if (!firstResult)
    return createError(firstResult.error());
  auto firstType = firstResult.value();
  if (insideDeclWithGenerics &&
      typeTable_->isGenericType(firstType, currentGenericList_))
    firstType = std::make_shared<PlaceHolder>(firstType);

  // Evaluar la rama 'second'
  auto secondResult = node->second()->accept(*this);
  if (!secondResult)
    return createError(secondResult.error());
  auto secondType = secondResult.value();
  if (insideDeclWithGenerics &&
      typeTable_->isGenericType(secondType, currentGenericList_))
    secondType = std::make_shared<PlaceHolder>(secondType);

  if (typeTable_->areSameType(firstType, secondType))
    return firstType;
  else if (!typeTable_->haveCommonAncestor(firstType, secondType))
    return createError(ERROR_TYPE::TYPE,
                       "inconsistent types in ternary operator");
  else
    return firstType;
}

/*
- debe retornar bool
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_CONDITION *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_CONDITION");
  }
  auto condResult = node->condition()->accept(*this);
  if (!condResult)
    return createError(condResult.error());
  auto condType = condResult.value();

  if (auto constCond = std::dynamic_pointer_cast<ConstType>(condType))
    condType = constCond->baseType();

  if (insideDeclWithGenerics &&
      typeTable_->isGenericType(condType, currentGenericList_))
    return std::make_shared<PlaceHolder>(condType);

  auto boolType = typeTable_->getType("bool");
  auto intType = typeTable_->getType("int");
  auto charType = typeTable_->getType("char");
  bool isEnum = (std::dynamic_pointer_cast<EnumType>(condType) != nullptr);

  if (!typeTable_->areSameType(condType, *boolType) &&
      !typeTable_->areSameType(condType, *intType) &&
      !typeTable_->areSameType(condType, *charType) && !isEnum) {
    return createError(ERROR_TYPE::TYPE,
                       "condition must be bool, int, char, or enum");
  }

  return condType;
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

  std::vector<std::shared_ptr<Type>> argTypes;
  for (const auto &expr : node->parameters()) {
    auto res = expr->accept(*this);
    if (!res)
      return createError(res.error());
    argTypes.push_back(res.value());
  }

  auto candidates = functionTable_->getFunctions(node->id());
  if (candidates.empty())
    return createError(ERROR_TYPE::FUNCTION,
                       "no function with id: " + node->id() + " exists");

  std::vector<Function> viableFunctions;
  const auto &explicitGenerics = node->replaceOfGenerics();

  for (const auto &func : candidates) {
    if (func.params().size() != argTypes.size())
      continue;
    if (!explicitGenerics.empty() &&
        func.generics().size() != explicitGenerics.size())
      continue;

    bool candidateMatches = true;
    const auto &funcParams = func.params().params();
    for (size_t i = 0; i < funcParams.size(); ++i) {
      auto paramType = funcParams[i].second;
      auto argType = argTypes[i];

      if (insideDeclWithGenerics &&
          (typeTable_->isGenericType(paramType, currentGenericList_) ||
           typeTable_->isGenericType(argType, currentGenericList_))) {
        continue;
      }
      if (!typeTable_->canAssign(paramType, argType)) {
        candidateMatches = false;
        break;
      }
    }
    if (candidateMatches)
      viableFunctions.push_back(func);
  }

  if (viableFunctions.empty())
    return createError(ERROR_TYPE::FUNCTION,
                       "no matching function found for call: " + node->id());
  if (viableFunctions.size() > 1)
    return createError(ERROR_TYPE::FUNCTION,
                       "ambiguous function call for: " + node->id());
  currentType_ = viableFunctions.front().returnType();
  return viableFunctions.front().returnType();
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

  if (!node->generics().empty()) {
    insideDeclWithGenerics = true;
    currentGenericList_ = node->generics();
  }

  auto bodyRes = node->body()->accept(*this);
  if (!bodyRes)
    return createError(bodyRes.error());
  auto bodyType = bodyRes.value();

  if (!typeTable_->areSameType(bodyType, typeTable_->noPropagateType())) {
    if (!typeTable_->canAssign(node->returnType(), bodyType))
      return createError(
          ERROR_TYPE::TYPE,
          "function body return type does not match declared return type");
  }

  insideDeclWithGenerics = false;
  currentGenericList_.clear();
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
  if (!node->expression())
    return *typeTable_->getType("void");
  auto result = node->expression()->accept(*this);
  if (!result)
    return createError(result.error());
  auto retType = result.value();
  if (insideDeclWithGenerics &&
      typeTable_->isGenericType(retType, currentGenericList_))
    return std::make_shared<PlaceHolder>(retType);
  return retType;
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
  if (!node->generics().empty()) {
    insideDeclWithGenerics = true;
    currentGenericList_ = node->generics();
  }

  analyzingInsideClass = true;

  if (node->constructor()) {
    auto consRes = node->constructor()->accept(*this);
    if (!consRes)
      return createError(consRes.error());
  }

  if (node->destructor()) {
    auto destrRes = node->destructor()->accept(*this);
    if (!destrRes)
      return createError(destrRes.error());
  }

  for (const auto &method : node->methods()) {
    auto methodRes = method->accept(*this);
    if (!methodRes)
      return createError(methodRes.error());
  }

  insideDeclWithGenerics = false;
  currentGenericList_.clear();
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
  if (!currentType_)
    return createError(ERROR_TYPE::TYPE,
                       "current type not set for attribute access");

  auto baseType = currentType_;
  if (insideDeclWithGenerics &&
      typeTable_->isGenericType(baseType, currentGenericList_))
    return std::make_shared<PlaceHolder>(baseType);

  auto userType = std::dynamic_pointer_cast<UserType>(baseType);
  if (!userType)
    return createError(ERROR_TYPE::TYPE,
                       "attribute access requires a user-defined type");

  auto attrRes = userType->getAttribute(node->id());
  if (!attrRes)
    return createError(attrRes.error());
  auto attrType = attrRes.value().type();

  if (insideDeclWithGenerics &&
      typeTable_->isGenericType(attrType, currentGenericList_))
    attrType = std::make_shared<PlaceHolder>(attrType);

  currentType_ = attrType;
  return attrType;
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

  std::vector<std::shared_ptr<Type>> argTypes;
  for (const auto &expr : node->parameters()) {
    auto res = expr->accept(*this);
    if (!res)
      return createError(res.error());
    argTypes.push_back(res.value());
  }

  if (!currentType_)
    return createError(ERROR_TYPE::TYPE,
                       "current type not set for method call");

  auto userType = std::dynamic_pointer_cast<UserType>(currentType_);
  if (!userType)
    return createError(ERROR_TYPE::TYPE,
                       "method call requires a user-defined type");

  auto methodsExp = userType->getMethods(node->id());
  if (!methodsExp)
    return createError(methodsExp.error());
  auto methods = methodsExp.value();
  if (methods.empty())
    return createError(ERROR_TYPE::METHOD,
                       "no method with id: " + node->id() + " exists");

  std::vector<Method> viableMethods;
  const auto &explicitGenerics = node->replaceOfGenerics();

  for (const auto &method : methods) {
    // Check if method generics match the explicit generics, if provided.
    if (!explicitGenerics.empty() &&
        method.generics().size() != explicitGenerics.size())
      continue;
    // Check parameter count.
    if (method.params().size() != argTypes.size())
      continue;
    bool candidateMatches = true;
    const auto &params = method.params().params();
    for (size_t i = 0; i < params.size(); ++i) {
      auto paramType = params[i].second;
      auto argType = argTypes[i];
      // For generics, if either the parameter or argument type is still
      // generic, we skip the strict check here.
      if (insideDeclWithGenerics &&
          (typeTable_->isGenericType(paramType, currentGenericList_) ||
           typeTable_->isGenericType(argType, currentGenericList_))) {
        continue;
      }
      if (!typeTable_->canAssign(paramType, argType)) {
        candidateMatches = false;
        break;
      }
    }
    if (candidateMatches)
      viableMethods.push_back(method);
  }

  if (viableMethods.empty())
    return createError(ERROR_TYPE::METHOD,
                       "no matching method found for call: " + node->id());
  if (viableMethods.size() > 1)
    return createError(ERROR_TYPE::METHOD,
                       "ambiguous method call for: " + node->id());

  return viableMethods.front().returnType();
}

/*
- igual que funcion
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const AST_METHOD_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_METHOD_DECL");
  }
  if (!node->generics().empty()) {
    insideDeclWithGenerics = true;
    currentGenericList_ = node->generics();
  }

  auto bodyRes = node->body()->accept(*this);
  if (!bodyRes)
    return createError(bodyRes.error());
  auto bodyType = bodyRes.value();

  if (!typeTable_->areSameType(bodyType, typeTable_->noPropagateType()) &&
      !typeTable_->areSameType(bodyType, typeTable_->breakType())) {
    if (!typeTable_->canAssign(node->returnType(), bodyType))
      return createError(
          ERROR_TYPE::TYPE,
          "method body return type does not match declared return type");
  }

  insideDeclWithGenerics = false;
  currentGenericList_.clear();
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

  if (!node->generics().empty()) {
    insideDeclWithGenerics = true;
    currentGenericList_ = node->generics();
  }

  auto bodyRes = node->body()->accept(*this);
  if (!bodyRes)
    return createError(bodyRes.error());
  auto bodyType = bodyRes.value();

  if (!typeTable_->areSameType(bodyType, typeTable_->noPropagateType()) &&
      !typeTable_->areSameType(bodyType, typeTable_->breakType()))
    return createError(ERROR_TYPE::TYPE,
                       "constructor body must not return a value");

  insideDeclWithGenerics = false;
  currentGenericList_.clear();
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
    auto res = arg->accept(*this);
    if (!res)
      return createError(res.error());
  }

  std::vector<std::shared_ptr<Type>> processedReplacements;
  for (const auto &rep : node->replacements()) {
    std::shared_ptr<Type> proc = rep;
    if (insideDeclWithGenerics &&
        typeTable_->isGenericType(rep, currentGenericList_))
      proc = std::make_shared<PlaceHolder>(rep);
    processedReplacements.push_back(proc);
  }

  auto fatherType = node->fatherType();
  if (!fatherType)
    return createError(ERROR_TYPE::TYPE,
                       "father type not specified in super call");

  if (!processedReplacements.empty()) {
    auto userType = std::dynamic_pointer_cast<UserType>(fatherType);
    if (!userType)
      return createError(
          ERROR_TYPE::TYPE,
          "father type in super call must be a user-defined type");
    return std::make_shared<GenericInstanceType>(userType,
                                                 processedReplacements);
  }

  return typeTable_->getType(fatherType->toString());
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
  if (!node->userType())
    return createError(ERROR_TYPE::TYPE, "user type not set for 'this'");

  auto typeFromTable = typeTable_->getType(node->userType()->name());
  if (!typeFromTable)
    return createError(typeFromTable.error());
  auto userType = std::dynamic_pointer_cast<UserType>(*typeFromTable);
  if (!userType)
    return createError(ERROR_TYPE::TYPE,
                       "'this' must refer to a user-defined type");

  auto ptrType = std::make_shared<PointerType>(userType);
  return std::make_shared<ConstType>(ptrType);
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
  std::vector<std::shared_ptr<Type>> argTypes;
  for (const auto &expr : node->parameters()) {
    auto res = expr->accept(*this);
    if (!res)
      return createError(res.error());
    argTypes.push_back(res.value());
  }

  auto baseTypeExp = typeTable_->getType(node->id());
  if (!baseTypeExp)
    return createError(baseTypeExp.error());
  auto baseType = baseTypeExp.value();

  auto userType = std::dynamic_pointer_cast<UserType>(baseType);
  if (!userType)
    return createError(ERROR_TYPE::TYPE,
                       "constructor call id must refer to a user-defined type");

  auto cons = userType->constructor();
  if (!cons)
    return createError(ERROR_TYPE::FUNCTION,
                       "no constructor defined for type: " + userType->name());

  if (cons->params().size() != argTypes.size())
    return createError(ERROR_TYPE::FUNCTION,
                       "constructor parameter count mismatch for type: " +
                           userType->name());

  const auto &consParams = cons->params().params();
  for (size_t i = 0; i < consParams.size(); ++i) {
    auto paramType = consParams[i].second;
    auto argType = argTypes[i];
    if (!typeTable_->canAssign(paramType, argType))
      return createError(ERROR_TYPE::TYPE,
                         "argument " + std::to_string(i) +
                             " type mismatch in constructor call for type: " +
                             userType->name());
  }

  if (!node->replaceOfGenerics().empty()) {
    std::vector<std::shared_ptr<Type>> genericArgs;
    for (const auto &gen : node->replaceOfGenerics()) {
      if (insideDeclWithGenerics &&
          typeTable_->isGenericType(gen, currentGenericList_))
        genericArgs.push_back(std::make_shared<PlaceHolder>(gen));
      else
        genericArgs.push_back(gen);
    }
    return std::make_shared<GenericInstanceType>(userType, genericArgs);
  }

  return baseType;
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
  auto exprTypeRes = node->value()->accept(*this);
  if (!exprTypeRes)
    return createError(exprTypeRes.error());
  auto deducedType = exprTypeRes.value();

  if (insideDeclWithGenerics &&
      typeTable_->isGenericType(deducedType, currentGenericList_))
    deducedType = std::make_shared<PlaceHolder>(deducedType);

  auto insertRes =
      currentScope_->insert(Variable{node->id(), deducedType, nullptr});
  if (!insertRes)
    return createError(insertRes.error());

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
  auto declaredType = node->varType();
  if (insideDeclWithGenerics &&
      typeTable_->isGenericType(declaredType, currentGenericList_))
    declaredType = std::make_shared<PlaceHolder>(declaredType);

  auto valueRes = node->value()->accept(*this);
  if (!valueRes)
    return createError(valueRes.error());
  auto valueType = valueRes.value();

  if (!typeTable_->canAssign(declaredType, valueType))
    return createError(
        ERROR_TYPE::TYPE,
        "assigned value type does not match declared variable type");

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

  if (analyzingInsideClass && currentUserType_ &&
      currentUserType_->hasAttribute(node->id())) {
    auto attrExp = currentUserType_->getAttribute(node->id());
    if (!attrExp)
      return createError(attrExp.error());
    return attrExp.value().type();
  }

  if (!currentScope_->has(node->id()))
    return createError(ERROR_TYPE::VARIABLE,
                       "variable: " + node->id() + " does not exist");
  auto varExp = currentScope_->getVariable(node->id());
  if (!varExp)
    return createError(varExp.error());
  return varExp.value().type();
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

/*
- comprueba que no hubo errores
- retorna int / void
*/
std::expected<std::shared_ptr<Type>, Error>
TypeAnalysis::visit(const Tree *tree) const noexcept {
  if (!tree) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid Tree");
  }
  auto result = tree->root()->accept(*this);
  if (!result)
    return createError(result.error());

  auto bodyType = result.value();
  auto intTypeExp = typeTable_->getType("int");
  auto voidTypeExp = typeTable_->getType("void");
  if (!intTypeExp || !voidTypeExp)
    return createError(ERROR_TYPE::TYPE, "failed to retrieve int or void type");

  auto intType = intTypeExp.value();
  auto voidType = voidTypeExp.value();
  if (!(typeTable_->areSameType(bodyType, intType) ||
        typeTable_->areSameType(bodyType, voidType)))
    return createError(ERROR_TYPE::TYPE, "body must return int or void, got " +
                                             bodyType->toString());

  return result;
}

} // namespace nicole
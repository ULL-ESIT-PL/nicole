#include "../../../inc/visitors/fillSemanticInfo/fillSemanticInfo.h"
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

void FillSemanticInfo::pushScope() const noexcept {
  auto newScope = std::make_shared<Scope>(currentScope_);
  currentScope_ = newScope;
  if (!firstScope_) {
    firstScope_ = currentScope_;
  }
}

void FillSemanticInfo::popScope() const noexcept {
  if (currentScope_) {
    currentScope_ = currentScope_->father();
  }
}

std::expected<std::vector<GenericParameter>, Error>
FillSemanticInfo::mergeGenericLists(
    const std::vector<GenericParameter> &list,
    const std::vector<GenericParameter> &list1) const noexcept {
  if (hasDuplicatedGenerics(list)) {
    return createError(ERROR_TYPE::TYPE, "has duplicated generics");
  }
  if (hasDuplicatedGenerics(list1)) {
    return createError(ERROR_TYPE::TYPE, "has duplicated generics");
  }
  std::vector<GenericParameter> result{};
  result.insert(result.end(), list.begin(), list.end());
  result.insert(result.end(), list1.begin(), list1.end());
  if (hasDuplicatedGenerics(result)) {
    return createError(ERROR_TYPE::TYPE, "has duplicated generics");
  }
  return result;
}

bool FillSemanticInfo::hasDuplicatedGenerics(
    const std::vector<GenericParameter> &list) const noexcept {
  std::unordered_set<GenericParameter> set(list.begin(), list.end());
  return set.size() != list.size();
}

bool FillSemanticInfo::areAmbiguousFunctions(
    const Function &first, const Function &second) const noexcept {
  if (first.id() != second.id()) {
    return false;
  }
  if (first.generics().size() != second.generics().size()) {
    return false;
  }
  if (first.params().size() != second.params().size()) {
    return false;
  }
  const auto parameters = first.params().params();
  const auto parametersOther = second.params().params();
  for (size_t i = 0; i < parameters.size(); ++i) {
    if (!typeTable_->areSameType(parameters[i].second,
                                 parametersOther[i].second)) {
      if (!(typeTable_->isGenericType(parameters[i].second, first.generics()) &&
            typeTable_->isGenericType(parametersOther[i].second,
                                      second.generics()))) {
        return false;
      }
    }
  }
  return true;
}

bool FillSemanticInfo::areAmbiguousMethods(
    const Method &first, const Method &second) const noexcept {
  if (first.id() != second.id()) {
    return false;
  }
  if (first.generics().size() != second.generics().size()) {
    return false;
  }
  if (first.params().size() != second.params().size()) {
    return false;
  }
  const auto parameters = first.params().params();
  const auto parametersOther = second.params().params();
  for (size_t i = 0; i < parameters.size(); ++i) {
    if (!typeTable_->areSameType(parameters[i].second,
                                 parametersOther[i].second)) {
      if (!(typeTable_->isGenericType(parameters[i].second, first.generics()) &&
            typeTable_->isGenericType(parametersOther[i].second,
                                      second.generics()))) {
        return false;
      }
    }
  }
  return true;
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_BOOL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_BOOL");
  }
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_CHAR *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_CHAR");
  }
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_DOUBLE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DOUBLE");
  }
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_FLOAT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_FLOAT");
  }
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_INT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_INT");
  }
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_NULL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_NULL");
  }
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_STRING *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_STRING");
  }
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_VECTOR *node) const noexcept {
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

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_INDEX *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_INDEX");
  }
  return node->index()->accept(*this);
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_DELETE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DELETE");
  }
  return node->value()->accept(*this);
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_NEW *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_NEW");
  }
  return node->value()->accept(*this);
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_DEREF *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DEREF");
  }
  return node->value()->accept(*this);
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_BINARY *node) const noexcept {
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

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_UNARY *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_UNARY");
  }
  return node->value()->accept(*this);
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_ASSIGNMENT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ASSIGNMENT");
  }
  const auto left{node->left()->accept(*this)};
  if (!left) {
    return createError(left.error());
  }
  const auto value{node->value()->accept(*this)};
  if (!value) {
    return createError(value.error());
  }
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_PRINT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_PRINT");
  }
  for (const auto &expr : node->values()) {
    const auto result{expr->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_IMPORT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_IMPORT");
  }
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_STATEMENT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_STATEMENT");
  }
  return node->expression()->accept(*this);
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_BODY *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_BODY");
  }
  for (const auto &expr : node->body()) {
    const auto result{expr->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_WHILE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_WHILE");
  }
  const auto condition{node->condition()->accept(*this)};
  if (!condition) {
    return createError(condition.error());
  }
  pushScope();
  node->body()->setScope(currentScope_);
  const auto body{node->body()->accept(*this)};
  if (!body) {
    return createError(body.error());
  }
  popScope();
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_FOR *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_FOR");
  }
  pushScope();
  node->body()->setScope(currentScope_);
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
  popScope();
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_DO_WHILE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DO_WHILE");
  }
  pushScope();
  node->body()->setScope(currentScope_);
  const auto body{node->body()->accept(*this)};
  if (!body) {
    return createError(body.error());
  }
  popScope();
  const auto condition{node->condition()->accept(*this)};
  if (!condition) {
    return createError(condition.error());
  }
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_PASS *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_PASS");
  }
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_STOP *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_STOP");
  }
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_IF *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_IF");
  }
  const auto condition{node->condition()->accept(*this)};
  if (!condition) {
    return createError(condition.error());
  }
  pushScope();
  node->body()->setScope(currentScope_);
  const auto body{node->body()->accept(*this)};
  if (!body) {
    return createError(body.error());
  }
  popScope();
  for (const auto &elseIf : node->elseIf()) {
    const auto result{elseIf->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }
  if (node->elseBody()) {
    pushScope();
    node->elseBody()->setScope(currentScope_);
    const auto elseBody{node->elseBody()->accept(*this)};
    if (!elseBody) {
      return createError(elseBody.error());
    }
    popScope();
  }
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_ELSE_IF *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ELSE_IF");
  }
  const auto condition{node->condition()->accept(*this)};
  if (!condition) {
    return createError(condition.error());
  }
  pushScope();
  node->body()->setScope(currentScope_);
  const auto body{node->body()->accept(*this)};
  if (!body) {
    return createError(body.error());
  }
  popScope();
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_SWITCH *node) const noexcept {
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

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_CASE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_CASE");
  }
  const auto match{node->match()->accept(*this)};
  if (!match) {
    return createError(match.error());
  }
  pushScope();
  node->body()->setScope(currentScope_);
  const auto body{node->body()->accept(*this)};
  if (!body) {
    return createError(body.error());
  }
  popScope();
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_DEFAULT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DEFAULT");
  }
  pushScope();
  node->body()->setScope(currentScope_);
  const auto body{node->body()->accept(*this)};
  if (!body) {
    return createError(body.error());
  }
  popScope();
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_TERNARY *node) const noexcept {
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

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_CONDITION *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_CONDITION");
  }
  return node->condition()->accept(*this);
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_FUNC_CALL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_FUNC_CALL");
  }

  if (functionTable_->getFunctions(node->id()).empty()) {
    return createError(ERROR_TYPE::FUNCTION,
                       "no function with id: " + node->id() + " exists");
  }

  for (const auto &replacement : node->replaceOfGenerics()) {
    if (!typeTable_->isPossibleType(replacement) and
        !typeTable_->isGenericType(replacement, currentGenericList_)) {
      return createError(ERROR_TYPE::TYPE,
                         replacement->toString() +
                             " is not a posibble type or generic");
    }
  }

  for (const auto &expr : node->parameters()) {
    const auto resul{expr->accept(*this)};
    if (!resul) {
      return createError(resul.error());
    }
  }
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_FUNC_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_FUNC_DECL");
  }

  const Function newFunction{Function{node->id(), node->generics(),
                                      node->parameters(), node->returnType(),
                                      node->body()}};

  const auto functions{functionTable_->getFunctions(newFunction.id())};
  for (const auto &func : functions) {
    if (areAmbiguousFunctions(newFunction, func)) {
      return createError(ERROR_TYPE::FUNCTION,
                         "redeclaration of function: " + newFunction.id());
    }
  }

  functionTable_->insert(newFunction);

  pushScope();
  node->body()->setScope(currentScope_);

  currentGenericList_ = node->generics();
  if (hasDuplicatedGenerics(currentGenericList_)) {
    return createError(ERROR_TYPE::FUNCTION, "has duplicated generics");
  }

  for (const auto &param : node->parameters()) {
    if (!typeTable_->isPossibleType(param.second) and
        !typeTable_->isGenericType(param.second, currentGenericList_)) {
      return createError(ERROR_TYPE::TYPE,
                         param.second->toString() +
                             " is not a posibble type or generic");
    }
    const auto insertVariable{
        currentScope_->insert(Variable{param.first, param.second, nullptr})};
    if (!insertVariable) {
      return createError(insertVariable.error());
    }
  }

  if (!typeTable_->isPossibleType(node->returnType()) and
      !typeTable_->isGenericType(node->returnType(), currentGenericList_)) {
    return createError(ERROR_TYPE::TYPE,
                       node->returnType()->toString() +
                           " is not a posibble type or generic");
  }

  const auto body{node->body()->accept(*this)};
  if (!body) {
    return createError(body.error());
  }

  popScope();

  currentGenericList_.clear();

  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_RETURN *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_RETURN");
  }
  if (!node->expression()) {
    return {};
  }
  return node->expression()->accept(*this);
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_ENUM *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ENUM");
  }
  const auto insert{typeTable_->insert(
      std::make_shared<EnumType>(node->id(), node->identifiers()))};
  if (!insert) {
    return createError(insert.error());
  }
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_ENUM_ACCESS *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ENUM_ACCESS");
  }
  const auto typeExists{typeTable_->getType(node->enumId())};
  if (!typeExists) {
    return createError(typeExists.error());
  }
  const auto enumType{std::dynamic_pointer_cast<EnumType>(*typeExists)};
  if (!enumType) {
    return createError(
        ERROR_TYPE::TYPE,
        "attempting to access a type as a enum but it isn't enum");
  }
  if (!enumType->hasIdentifier(node->identifier())) {
    return createError(ERROR_TYPE::TYPE, "the enum " + enumType->name() +
                                             " does not have the identifier " +
                                             node->identifier());
  }
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_STRUCT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_STRUCT");
  }

  analyzingInsideClass = true;

  currentStructGenericList_ = node->generics();

  if (hasDuplicatedGenerics(currentStructGenericList_)) {
    return createError(ERROR_TYPE::TYPE, "has duplicated generics");
  }

  std::shared_ptr<UserType> father{nullptr};
  if (node->fatherType()) {
    const auto instanceType =
        std::dynamic_pointer_cast<GenericInstanceType>(node->fatherType());
    if (instanceType) {
      if (typeTable_->isGenericType(instanceType, currentStructGenericList_)) {
        return createError(ERROR_TYPE::TYPE,
                           instanceType->toString() +
                               " is a generic so you cannot extend from it");
      }
      auto fatherExpected = typeTable_->getType(instanceType->name());
      if (!fatherExpected) {
        return createError(fatherExpected.error());
      }
      father = std::dynamic_pointer_cast<UserType>(fatherExpected.value());
      if (!father) {
        return createError(ERROR_TYPE::TYPE,
                           "The father type is not a UserType");
      }
    } else {
      const auto userType =
          std::dynamic_pointer_cast<UserType>(node->fatherType());
      if (!userType) {
        return createError(
            ERROR_TYPE::TYPE,
            "father type can only be a user type or generic instance");
      }
      if (typeTable_->isGenericType(userType, currentStructGenericList_)) {
        return createError(ERROR_TYPE::TYPE,
                           userType->toString() +
                               " is a generic so you cannot extend from it");
      }
      auto fatherExpected = typeTable_->getType(userType->name());
      if (!fatherExpected) {
        return createError(fatherExpected.error());
      }
      father = std::dynamic_pointer_cast<UserType>(fatherExpected.value());
      if (!father) {
        return createError(ERROR_TYPE::TYPE,
                           "The father type is not a UserType");
      }
    }
  }

  currentUserType_ =
      std::make_shared<UserType>(node->id(), father, node->generics());

  const auto insertType{typeTable_->insert(currentUserType_)};
  if (!insertType) {
    return createError(insertType.error());
  }

  pushScope();

  size_t pos{0};
  for (const auto &attr : node->attributes()) {
    const auto insert{
        currentUserType_->insertAttr(Attribute{attr.first, attr.second, pos})};
    if (!insert) {
      return createError(insert.error());
    }
    ++pos;
  }

  for (const auto &method : node->methods()) {
    const auto result{method->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }

  const auto constructor{node->constructor()->accept(*this)};
  if (!constructor) {
    return createError(constructor.error());
  }

  const auto destructor{node->destructor()->accept(*this)};
  if (!destructor) {
    return createError(destructor.error());
  }

  popScope();

  analyzingInsideClass = false;
  currentStructGenericList_.clear();
  currentGenericList_.clear();

  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_ATTR_ACCESS *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ATTR_ACCESS");
  }
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_METHOD_CALL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_METHOD_CALL");
  }

  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_METHOD_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_METHOD_DECL");
  }

  currentGenericList_ = node->generics();

  const auto genrics{
      mergeGenericLists(currentStructGenericList_, currentGenericList_)};
  if (!genrics) {
    return createError(genrics.error());
  }

  currentGenericList_ = *genrics;

  Method newMethod{node->id(),         currentGenericList_, node->parameters(),
                   node->returnType(), node->body(),        node->isVirtual()};

  const auto combinedMethods{currentUserType_->getMethods(node->id())};
  if (combinedMethods) {
    for (const auto &existingMethod : *combinedMethods) {
      if (areAmbiguousMethods(existingMethod, newMethod)) {
        if (!existingMethod.isInherited()) {
          return createError(ERROR_TYPE::METHOD,
                             "Ambiguous method declaration for: " + node->id());
        }
        if (existingMethod.isInherited() && !existingMethod.isVirtual()) {
          return createError(ERROR_TYPE::METHOD,
                             "Ambiguous method declaration for: " + node->id());
        }
      }
    }
  }

  currentUserType_->insertMethod(newMethod);

  pushScope();
  node->body()->setScope(currentScope_);

  for (const auto &param : node->parameters()) {
    if (currentUserType_->hasAttribute(param.first)) {
      return createError(ERROR_TYPE::ATTR, " the variable " + param.first +
                                               " is shadowing the atribute");
    }

    if (!typeTable_->isPossibleType(param.second) and
        !typeTable_->isGenericType(param.second, currentGenericList_)) {
      return createError(ERROR_TYPE::TYPE,
                         param.second->toString() +
                             " is not a posibble type or generic");
    }

    const auto insert{
        currentScope_->insert(Variable{param.first, param.second, nullptr})};
    if (!insert) {
      return createError(insert.error());
    }
  }

  if (!typeTable_->isPossibleType(node->returnType()) and
      !typeTable_->isGenericType(node->returnType(), currentGenericList_)) {
    return createError(ERROR_TYPE::TYPE,
                       node->returnType()->toString() +
                           " is not a posibble type or generic");
  }

  const auto body{node->body()->accept(*this)};
  if (!body) {
    return createError(body.error());
  }

  popScope();

  currentGenericList_.clear();

  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_CONSTRUCTOR_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_CONSTRUCTOR_DECL");
  }

  currentGenericList_ = node->generics();

  const auto genrics{
      mergeGenericLists(currentStructGenericList_, currentGenericList_)};
  if (!genrics) {
    return createError(genrics.error());
  }

  currentGenericList_ = *genrics;

  currentUserType_->setConstructor(std::make_shared<Constructor>(
      node->id(), node->generics(), node->parameters(), node->returnType(),
      node->body()));

  pushScope();
  node->body()->setScope(currentScope_);

  for (const auto &param : node->parameters()) {
    if (currentUserType_->hasAttribute(param.first)) {
      return createError(ERROR_TYPE::ATTR, " the variable " + param.first +
                                               " is shadowing the atribute");
    }

    if (!typeTable_->isPossibleType(param.second) and
        !typeTable_->isGenericType(param.second, currentGenericList_)) {
      return createError(ERROR_TYPE::TYPE,
                         param.second->toString() +
                             " is not a posibble type or generic");
    }

    const auto insert{
        currentScope_->insert(Variable{param.first, param.second, nullptr})};
    if (!insert) {
      return createError(insert.error());
    }
  }

  if (node->super()) {
    const auto result{node->super()->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }

  if (!typeTable_->isPossibleType(node->returnType()) and
      !typeTable_->isGenericType(node->returnType(), currentGenericList_)) {
    return createError(ERROR_TYPE::TYPE,
                       node->returnType()->toString() +
                           " is not a posibble type or generic");
  }

  const auto body{node->body()->accept(*this)};
  if (!body) {
    return createError(body.error());
  }

  popScope();

  currentGenericList_.clear();

  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_SUPER *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_SUPER");
  }
  for (const auto &replacement : node->replacements()) {
    if (!typeTable_->isPossibleType(replacement) and
        !typeTable_->isGenericType(replacement, currentGenericList_)) {
      return createError(ERROR_TYPE::TYPE,
                         replacement->toString() +
                             " is not a posibble type or generic");
    }
  }
  for (const auto &arg : node->arguments()) {
    const auto result{arg->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_DESTRUCTOR_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_DESTRUCTOR_DECL");
  }

  currentUserType_->setDestructor(
      std::make_shared<Destructor>(node->id(), node->body()));

  pushScope();
  node->body()->setScope(currentScope_);

  const auto body{node->body()->accept(*this)};
  if (!body) {
    return createError(body.error());
  }

  popScope();

  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_THIS *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_THIS");
  }
  if (analyzingInsideClass) {
    node->setUserType(currentUserType_);
  }
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_CONSTRUCTOR_CALL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_CONSTRUCTOR_CALL");
  }
  if (!typeTable_->getType(node->id())) {
    return createError(ERROR_TYPE::TYPE,
                       "no type with id: " + node->id() + " exists");
  }

  for (const auto &replacement : node->replaceOfGenerics()) {
    if (!typeTable_->isPossibleType(replacement) and
        !typeTable_->isGenericType(replacement, currentGenericList_)) {
      return createError(ERROR_TYPE::TYPE,
                         replacement->toString() +
                             " is not a posibble type or generic");
    }
  }

  for (const auto &expr : node->parameters()) {
    const auto resul{expr->accept(*this)};
    if (!resul) {
      return createError(resul.error());
    }
  }
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_AUTO_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_AUTO_DECL");
  }

  const auto value{node->value()->accept(*this)};
  if (!value) {
    return createError(value.error());
  }

  if (analyzingInsideClass and currentUserType_->hasAttribute(node->id())) {
    return createError(ERROR_TYPE::ATTR, " the variable " + node->id() +
                                             " is shadowing the atribute");
  }

  const auto insert{
      currentScope_->insert(Variable{node->id(), nullptr, nullptr})};
  if (!insert) {
    return createError(insert.error());
  }

  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_VAR_TYPED_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_VAR_TYPED_DECL");
  }

  const auto value{node->value()->accept(*this)};
  if (!value) {
    return createError(value.error());
  }

  if (analyzingInsideClass and currentUserType_->hasAttribute(node->id())) {
    return createError(ERROR_TYPE::ATTR, " the variable " + node->id() +
                                             " is shadowing the atribute");
  }

  if (!typeTable_->isPossibleType(node->varType()) and
      !typeTable_->isGenericType(node->varType(), currentGenericList_)) {
    return createError(ERROR_TYPE::TYPE,
                       node->varType()->toString() +
                           " is not a posibble type or generic");
  }

  const auto insert{
      currentScope_->insert(Variable{node->id(), node->varType(), nullptr})};
  if (!insert) {
    return createError(insert.error());
  }

  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_VAR_CALL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_VAR_CALL");
  }
  if (analyzingInsideClass and currentUserType_->hasAttribute(node->id())) {
    return {};
  }
  if (!currentScope_->has(node->id())) {
    return createError(ERROR_TYPE::VARIABLE,
                       "the variable " + node->id() + " does not exist");
  }
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_CHAINED *node) const noexcept {
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

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const Tree *tree) const noexcept {
  if (!tree) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid Tree");
  }
  pushScope();
  tree->root()->setScope(currentScope_);
  const auto result{tree->root()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  popScope();
  if (validateMode_) {
    const auto mainFunction{functionTable_->getFunctions("main")};
    if (mainFunction.empty() or mainFunction.size() > 1) {
      return createError(ERROR_TYPE::FUNCTION,
                         "if the validation is activated the user must "
                         "specifiy a main function and only one instance");
    }
    if (mainFunction[0].returnType()->toString() !=
        BasicType{BasicKind::Int}.toString()) {
      return createError(ERROR_TYPE::FUNCTION,
                         "if the validation is activated the user must "
                         "specifiy a main function and only one instance of it "
                         "that returns type int");
    }
  }
  return {};
}

} // namespace nicole
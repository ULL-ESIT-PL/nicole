#include "../../inc/visitors/validateTree.h"
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

#include "../../inc/parsingAnalysis/checkPosition.h"

namespace nicole {

std::expected<bool, Error>
ValidateTree::visit(const AST_BOOL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_BOOL");
  }
  if (CheckPosition::hasEveryAncestorInOrder(
          node, {AST_TYPE::STATEMENT, AST_TYPE::BODY})) {
    return createError(ERROR_TYPE::VALIDATE_TREE, "dangling bool");
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_CHAR *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_CHAR");
  }
  if (CheckPosition::hasEveryAncestorInOrder(
          node, {AST_TYPE::STATEMENT, AST_TYPE::BODY})) {
    return createError(ERROR_TYPE::VALIDATE_TREE, "dangling char");
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_DOUBLE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DOUBLE");
  }
  if (CheckPosition::hasEveryAncestorInOrder(
          node, {AST_TYPE::STATEMENT, AST_TYPE::BODY})) {
    return createError(ERROR_TYPE::VALIDATE_TREE, "dangling double");
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_FLOAT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_FLOAT");
  }
  if (CheckPosition::hasEveryAncestorInOrder(
          node, {AST_TYPE::STATEMENT, AST_TYPE::BODY})) {
    return createError(ERROR_TYPE::VALIDATE_TREE, "dangling float");
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_INT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_INT");
  }
  if (CheckPosition::hasEveryAncestorInOrder(
          node, {AST_TYPE::STATEMENT, AST_TYPE::BODY})) {
    return createError(ERROR_TYPE::VALIDATE_TREE, "dangling int");
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_NULL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_NULL");
  }
  if (CheckPosition::hasEveryAncestorInOrder(
          node, {AST_TYPE::STATEMENT, AST_TYPE::BODY})) {
    return createError(ERROR_TYPE::VALIDATE_TREE, "dangling null");
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_STRING *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_STRING");
  }
  if (CheckPosition::hasEveryAncestorInOrder(
          node, {AST_TYPE::STATEMENT, AST_TYPE::BODY})) {
    return createError(ERROR_TYPE::VALIDATE_TREE, "dangling string");
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_VECTOR *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_VECTOR");
  }
  if (CheckPosition::hasEveryAncestorInOrder(
          node, {AST_TYPE::STATEMENT, AST_TYPE::BODY})) {
    return createError(ERROR_TYPE::VALIDATE_TREE, "dangling vector");
  }
  for (const auto &value : node->values()) {
    const auto result{value->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }
  return true;
}

// chained
std::expected<bool, Error>
ValidateTree::visit(const AST_INDEX *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_INDEX");
  }
  if (!CheckPosition::hasAnyAncestorOf(node, {AST_TYPE::CHAIN})) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       "index operator can only appear in a chain expression");
  }
  const auto result{node->index()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  return true;
}

// statemetn / body / not null
std::expected<bool, Error>
ValidateTree::visit(const AST_DELETE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DELETE");
  }
  if (!CheckPosition::itsBodyAncestorHasParent(node)) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       "a delete statement must appear in scope");
  }
  const auto result{node->value()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_NEW *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_NEW");
  }
  if (CheckPosition::hasEveryAncestorInOrder(
          node, {AST_TYPE::STATEMENT, AST_TYPE::BODY})) {
    return createError(ERROR_TYPE::VALIDATE_TREE, "dangling new operator");
  }
  const auto result{node->value()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_DEREF *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DEREF");
  }
  if (CheckPosition::isOutOfScope(node)) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       "a deref operation cannot appear outside of a scope");
  }
  const auto result{node->value()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_BINARY *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ADD");
  }
  if (CheckPosition::isOutOfScope(node)) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       node->op().raw() +
                           " operation cannot appear outside of a scope, at " +
                           node->op().locInfo());
  }
  const auto left{node->left()->accept(*this)};
  if (!left) {
    return createError(left.error());
  }
  const auto right{node->right()->accept(*this)};
  if (!right) {
    return createError(right.error());
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_UNARY *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_NEG");
  }
  if (CheckPosition::isOutOfScope(node)) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       node->op().raw() +
                           " operation cannot appear outside of a scope");
  }
  const auto left{node->value()->accept(*this)};
  if (!left) {
    return createError(left.error());
  }
  return true;
}

// statemetn / body / not null or for
std::expected<bool, Error>
ValidateTree::visit(const AST_ASSIGNMENT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ASSIGNMENT");
  }
  if (!(CheckPosition::itsBodyAncestorHasParent(node) or
        CheckPosition::isInsideForHeader(node))) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       "a assignment can only exist "
                       "in a body or a for header init");
  }
  const auto left{node->left()->accept(*this)};
  if (!left) {
    return createError(left.error());
  }
  const auto right{node->value()->accept(*this)};
  if (!right) {
    return createError(right.error());
  }
  return true;
}

// statement / body / not null
std::expected<bool, Error>
ValidateTree::visit(const AST_PRINT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_PRINT");
  }
  if (!CheckPosition::itsBodyAncestorHasParent(node)) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       "a print statement only can appear in a scope");
  }
  for (const auto &chain : node->values()) {
    const auto result{chain->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }
  return true;
}

// statement / body / null
std::expected<bool, Error>
ValidateTree::visit(const AST_IMPORT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_IMPORT");
  }
  if (CheckPosition::itsBodyAncestorHasParent(node)) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       "a import must appear outside of any scope");
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_STATEMENT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_STATEMENT");
  }
  return node->expression()->accept(*this);
}

std::expected<bool, Error>
ValidateTree::visit(const AST_BODY *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_BODY");
  }
  for (const auto &statement : node->body()) {
    const auto val{statement->accept(*this)};
    if (!val) {
      return createError(val.error());
    }
  }
  return true;
}

// statement / body / not null
std::expected<bool, Error>
ValidateTree::visit(const AST_WHILE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_WHILE");
  }
  if (!CheckPosition::itsBodyAncestorHasParent(node)) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       "a while loop must appear in a scope");
  }
  const auto condition{node->condition()->accept(*this)};
  if (!condition) {
    return createError(condition.error());
  }
  const auto result{node->body()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  return true;
}

// statement / body / not null
std::expected<bool, Error>
ValidateTree::visit(const AST_FOR *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_FOR");
  }
  if (!CheckPosition::itsBodyAncestorHasParent(node)) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       "a for loop must appear in a scope");
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
  const auto result{node->body()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  return true;
}

// statement / body / not null
std::expected<bool, Error>
ValidateTree::visit(const AST_DO_WHILE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DO_WHILE");
  }
  if (!CheckPosition::itsBodyAncestorHasParent(node)) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       "a do while loop must appear in a scope");
  }
  const auto result{node->body()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  const auto condition{node->condition()->accept(*this)};
  if (!condition) {
    return createError(condition.error());
  }
  return true;
}

// do while / while / for
std::expected<bool, Error>
ValidateTree::visit(const AST_PASS *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_PASS");
  }
  if (!CheckPosition::hasAnyAncestorOf(
          node, {AST_TYPE::DO_WHILE, AST_TYPE::FOR, AST_TYPE::WHILE})) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       "a pass statement must appear inside a loop");
  }
  return true;
}

// do while / while / for
std::expected<bool, Error>
ValidateTree::visit(const AST_STOP *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_STOP");
  }
  if (!CheckPosition::hasAnyAncestorOf(node,
                                       {AST_TYPE::DO_WHILE, AST_TYPE::FOR,
                                        AST_TYPE::WHILE, AST_TYPE::SWITCH})) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       "a stop statement must appear inside a loop");
  }
  return true;
}

// statement / body / not null
std::expected<bool, Error>
ValidateTree::visit(const AST_IF *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_IF");
  }
  if (!CheckPosition::itsBodyAncestorHasParent(node)) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       "a if statement must appear inside of a scope");
  }
  const auto condition{node->condition()->accept(*this)};
  if (!condition) {
    return createError(condition.error());
  }
  const auto body{node->body()->accept(*this)};
  if (!body) {
    return createError(body.error());
  }
  for (const auto &elseif_ : node->elseIf()) {
    const auto result{elseif_->accept(*this)};
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
  return true;
}

// if
std::expected<bool, Error>
ValidateTree::visit(const AST_ELSE_IF *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ELSE_IF");
  }
  if (!CheckPosition::hasEveryAncestorInOrder(node, {AST_TYPE::IF})) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       "a else if must follow a if statement");
  }
  const auto condition{node->condition()->accept(*this)};
  if (!condition) {
    return createError(condition.error());
  }
  const auto result{node->body()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  return true;
}

// statement / body / not null
std::expected<bool, Error>
ValidateTree::visit(const AST_SWITCH *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_SWITCH");
  }
  if (!CheckPosition::itsBodyAncestorHasParent(node)) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       "a switch statement must appear inside of a scope");
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
  return true;
}

// switch
std::expected<bool, Error>
ValidateTree::visit(const AST_CASE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_CASE");
  }
  if (!CheckPosition::hasEveryAncestorInOrder(node, {AST_TYPE::SWITCH})) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       "a case statement can only appear in a switch scope");
  }
  const auto match{node->match()->accept(*this)};
  if (!match) {
    return createError(match.error());
  }
  const auto result{node->body()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  return true;
}

// switch
std::expected<bool, Error>
ValidateTree::visit(const AST_DEFAULT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DEFAULT");
  }
  if (!CheckPosition::hasEveryAncestorInOrder(node, {AST_TYPE::SWITCH})) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       "a default statement can only appear in a switch scope");
  }
  const auto result{node->body()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_TERNARY *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_TERNARY");
  }
  if (CheckPosition::isOutOfScope(node)) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       "a ternary operation cannot appear outside of a scope");
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
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_CONDITION *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_CONDITION");
  }
  if (CheckPosition::hasAnyAncestorOf(node, {AST_TYPE::IF, AST_TYPE::ELSE_IF,
                                             AST_TYPE::IF, AST_TYPE::SWITCH,
                                             AST_TYPE::DO_WHILE,
                                             AST_TYPE::WHILE, AST_TYPE::FOR})) {
    return createError(ERROR_TYPE::VALIDATE_TREE, "misplaced condition");
  }
  return node->condition()->accept(*this);
}

// chained
std::expected<bool, Error>
ValidateTree::visit(const AST_FUNC_CALL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_FUNC_CALL");
  }
  if (node->father() and node->father()->type() != AST_TYPE::CHAIN) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       "invalid hierachy AST_FUNC_CALL");
  }
  for (const auto &chain : node->parameters()) {
    const auto result{chain->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }
  return true;
}

// statement / body / null or struct / class
std::expected<bool, Error>
ValidateTree::visit(const AST_FUNC_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_FUNC_DECL");
  }
  if (CheckPosition::itsBodyAncestorHasParent(node)) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       "a funciton declaration cant appear inside a scope: " +
                           node->id());
  }
  const auto result{node->body()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  return true;
}

// func decl
std::expected<bool, Error>
ValidateTree::visit(const AST_RETURN *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_RETURN");
  }
  if (!CheckPosition::hasAnyAncestorOf(
          node, {AST_TYPE::FUNC_DECL, AST_TYPE::METHOD_DECL})) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       "a return declaration must has a function "
                       "declaration in its hierarchy");
  }
  const auto result{node->expression()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  return true;
}

// statement / body / null
std::expected<bool, Error>
ValidateTree::visit(const AST_ENUM *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ENUM");
  }
  if (CheckPosition::itsBodyAncestorHasParent(node)) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       "a enum declaration must be outside of any scope");
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_ENUM_ACCESS *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ENUM_ACCESS");
  }
  if (CheckPosition::hasEveryAncestorInOrder(
          node, {AST_TYPE::STATEMENT, AST_TYPE::BODY})) {
    return createError(ERROR_TYPE::VALIDATE_TREE, "dangling enum access");
  }
  return true;
}

// statement / body / null
std::expected<bool, Error>
ValidateTree::visit(const AST_STRUCT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_STRUCT");
  }
  if (CheckPosition::itsBodyAncestorHasParent(node)) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       "a struct declaration must be outside of any scope");
  }
  const auto constructor{node->constructor()->accept(*this)};
  if (!constructor) {
    return createError(constructor.error());
  }
  const auto destructor{node->destructor()->accept(*this)};
  if (!destructor) {
    return createError(destructor.error());
  }
  for (const auto &chain : node->methods()) {
    const auto result{chain->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }
  return true;
}

// chained childs
std::expected<bool, Error>
ValidateTree::visit(const AST_ATTR_ACCESS *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ATTR_ACCESS");
  }
  if (!CheckPosition::hasAnyAncestorOf(
          node, {AST_TYPE::ATTR_ACCESS, AST_TYPE::METHOD_CALL, AST_TYPE::INDEX,
                 AST_TYPE::CONSTRUCTOR_CALL, AST_TYPE::VAR_CALL})) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       "invalid hierarchy for attr access");
  }
  return true;
}

// chained childs
std::expected<bool, Error>
ValidateTree::visit(const AST_METHOD_CALL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_METHOD_CALL");
  }
  if (!CheckPosition::hasAnyAncestorOf(
          node, {AST_TYPE::ATTR_ACCESS, AST_TYPE::METHOD_CALL, AST_TYPE::INDEX,
                 AST_TYPE::CONSTRUCTOR_CALL, AST_TYPE::VAR_CALL})) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       "invalid hierarchy for Method call");
  }
  for (const auto &chain : node->parameters()) {
    const auto result{chain->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_METHOD_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_METHOD_DECL");
  }
  if (!CheckPosition::hasEveryAncestorInOrder(node, {AST_TYPE::STRUCT_DECL})) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       "invalid hierarchy for Method decl");
  }
  const auto result{node->body()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_CONSTRUCTOR_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_CONSTRUCTOR_DECL");
  }
  if (!CheckPosition::hasEveryAncestorInOrder(node, {AST_TYPE::STRUCT_DECL})) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       "invalid hierarchy for constructor decl");
  }
  if (node->super()) {
    const auto result{node->super()->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }
  const auto result{node->body()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_SUPER *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_SUPER");
  }
  if (!CheckPosition::hasEveryAncestorInOrder(node,
                                              {AST_TYPE::DESTRUCTOR_DECL})) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       "invalid hierarchy for destructor decl");
  }
  for (const auto &chain : node->arguments()) {
    const auto result{chain->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_DESTRUCTOR_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_DESTRUCTOR_DECL");
  }
  if (!CheckPosition::hasEveryAncestorInOrder(node, {AST_TYPE::STRUCT_DECL})) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       "invalid hierarchy for destructor decl");
  }
  const auto result{node->body()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  return true;
}

// func decl / struct
std::expected<bool, Error>
ValidateTree::visit(const AST_THIS *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_THIS");
  }
  if (!CheckPosition::hasAnyAncestorOf(node, {AST_TYPE::STRUCT_DECL})) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       "a this call can only appear in methods");
  }
  return true;
}

// chained
std::expected<bool, Error>
ValidateTree::visit(const AST_CONSTRUCTOR_CALL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_CONSTRUCTOR_CALL");
  }
  if (node->father()->type() != AST_TYPE::CHAIN) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       "invalid hierachy AST_VAR_CALL");
  }
  for (const auto &chain : node->parameters()) {
    const auto result{chain->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }
  return true;
}

// statement / body / not null or for
std::expected<bool, Error>
ValidateTree::visit(const AST_AUTO_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_AUTO_DECL");
  }
  if (!(CheckPosition::itsBodyAncestorHasParent(node) or
        CheckPosition::isInsideForHeader(node))) {
    return createError(
        ERROR_TYPE::VALIDATE_TREE,
        "a auto declaration can only exist in a body or a for header init");
  }
  const auto result{node->value()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  return true;
}

// statement / body / not null or for
std::expected<bool, Error>
ValidateTree::visit(const AST_VAR_TYPED_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_VAR_TYPED_DECL");
  }
  if (!(CheckPosition::itsBodyAncestorHasParent(node) or
        CheckPosition::isInsideForHeader(node))) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       "a var typed declaration can only exist in a body or a "
                       "for header init");
  }
  const auto result{node->value()->accept(*this)};
  if (!result) {
    return createError(result.error());
  }
  return true;
}

// chained
std::expected<bool, Error>
ValidateTree::visit(const AST_VAR_CALL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_VAR_CALL");
  }
  if (node->father() and node->father()->type() != AST_TYPE::CHAIN) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       "invalid hierachy AST_VAR_CALL");
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_CHAINED *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_CHAINED");
  }
  if (CheckPosition::isOutOfScope(node)) {
    return createError(ERROR_TYPE::VALIDATE_TREE,
                       "chained expressions cannot appear outside of scopes");
  }
  const auto base{node->base()->accept(*this)};
  if (!base) {
    return createError(base.error());
  }
  for (const auto &chain : node->operations()) {
    const auto result{chain->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const Tree *tree) const noexcept {
  if (!tree) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid Tree");
  }
  return tree->root()->accept(*this);
}

} // namespace nicole
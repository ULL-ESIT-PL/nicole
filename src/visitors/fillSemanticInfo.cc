#include "../../inc/visitors/fillSemanticInfo.h"
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

#include "../../inc/parsingAnalysis/ast/userTypes/ast_attrAccess.h"
#include "../../inc/parsingAnalysis/ast/userTypes/ast_constructorCall.h"
#include "../../inc/parsingAnalysis/ast/userTypes/ast_enum.h"
#include "../../inc/parsingAnalysis/ast/userTypes/ast_methodCall.h"
#include "../../inc/parsingAnalysis/ast/userTypes/ast_struct.h"
#include "../../inc/parsingAnalysis/ast/userTypes/ast_this.h"

#include "../../inc/parsingAnalysis/ast/variables/ast_autoDecl.h"
#include "../../inc/parsingAnalysis/ast/variables/ast_typedDecl.h"
#include "../../inc/parsingAnalysis/ast/variables/ast_varCall.h"

#include "../../inc/parsingAnalysis/ast/chained/ast_chained.h"

#include "../../inc/parsingAnalysis/ast/tree.h"
#include <memory>

namespace nicole {

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

  const auto exists{functionTable_->getFunctions(node->id())};
  if (!exists) {
    return createError(exists.error());
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
  const auto insertFunc{functionTable_->insert(Function{
      node->id(), node->generics(), node->parameters(), node->returnType()})};
  if (!insertFunc) {
    return createError(insertFunc.error());
  }

  pushScope();
  node->body()->setScope(currentScope_);
  for (const auto &param : node->parameters()) {
    const auto insertVar{currentScope_->insert(
        Variable{param.first, param.second, nullptr}, false)};
    if (!insertVar) {
      return createError(insertVar.error());
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
FillSemanticInfo::visit(const AST_RETURN *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_RETURN");
  }
  return node->expression()->accept(*this);
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_ENUM *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ENUM");
  }
  const auto insert{enumTable_->insert(Enum{node->id(), node->identifiers()})};
  if (!insert) {
    return createError(insert.error());
  }
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_STRUCT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_STRUCT");
  }
  pushScope();
  // node->body()->setScope(currentScope_);
  for (const auto &param : node->attributes()) {
    const auto insertVar{currentScope_->insert(
        Variable{param.first, param.second, nullptr}, true)};
    if (!insertVar) {
      return createError(insertVar.error());
    }
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
  for (const auto &expr : node->parameters()) {
    const auto result{expr->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_METHOD_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_METHOD_DECL");
  }
  pushScope();
  node->body()->setScope(currentScope_);
  for (const auto &param : node->parameters()) {
    const auto insertVar{currentScope_->insert(
        Variable{param.first, param.second, nullptr}, false)};
    if (!insertVar) {
      return createError(insertVar.error());
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
FillSemanticInfo::visit(const AST_CONSTRUCTOR_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_CONSTRUCTOR_DECL");
  }
  pushScope();
  node->body()->setScope(currentScope_);
  for (const auto &param : node->parameters()) {
    const auto insertVar{currentScope_->insert(
        Variable{param.first, param.second, nullptr}, false)};
    if (!insertVar) {
      return createError(insertVar.error());
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
FillSemanticInfo::visit(const AST_DESTRUCTOR_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_DESTRUCTOR_DECL");
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
FillSemanticInfo::visit(const AST_THIS *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_THIS");
  }
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_CONSTRUCTOR_CALL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_CONSTRUCTOR_CALL");
  }
  const auto type{typeTable_->getType(node->id())};
  if (!type) {
    return createError(type.error());
  }
  for (const auto &expr : node->parameters()) {
    const auto result{expr->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
  }
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_AUTO_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_AUTO_DECL");
  }
  const auto expr{node->value()->accept(*this)};
  if (!expr) {
    return createError(expr.error());
  }
  const auto insertVar{
      currentScope_->insert(Variable{node->id(), nullptr, nullptr}, false)};
  if (!insertVar) {
    return createError(insertVar.error());
  }
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_VAR_TYPED_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_VAR_TYPED_DECL");
  }
  const auto expr{node->value()->accept(*this)};
  if (!expr) {
    return createError(expr.error());
  }
  const auto insertVar{currentScope_->insert(
      Variable{node->id(), node->varType(), nullptr}, false)};
  if (!insertVar) {
    return createError(insertVar.error());
  }
  return {};
}

std::expected<std::monostate, Error>
FillSemanticInfo::visit(const AST_VAR_CALL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_VAR_CALL");
  }
  if (!currentScope_->has(node->id())) {
    return createError(ERROR_TYPE::VARIABLE,
                       "the variable: " + node->id() + " does not exist");
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
  return {};
}

} // namespace nicole
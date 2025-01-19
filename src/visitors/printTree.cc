#include "../../inc/visitors/printTree.h"
#include "../../inc/parsingAnalysis/ast/literals/ast_bool.h"
#include "../../inc/parsingAnalysis/ast/literals/ast_char.h"
#include "../../inc/parsingAnalysis/ast/literals/ast_double.h"
#include "../../inc/parsingAnalysis/ast/literals/ast_float.h"
#include "../../inc/parsingAnalysis/ast/literals/ast_int.h"
#include "../../inc/parsingAnalysis/ast/literals/ast_null.h"
#include "../../inc/parsingAnalysis/ast/literals/ast_string.h"

#include "../../inc/parsingAnalysis/ast/vector/ast_vector.h"

#include "../../inc/parsingAnalysis/ast/pointer/ast_delete.h"
#include "../../inc/parsingAnalysis/ast/pointer/ast_deref.h"
#include "../../inc/parsingAnalysis/ast/pointer/ast_new.h"
#include "../../inc/parsingAnalysis/ast/pointer/ast_ptr.h"

#include "../../inc/parsingAnalysis/ast/operators/binary/ast_add.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_and.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_bigger.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_biggerEqual.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_div.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_equal.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_module.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_mult.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_notEqual.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_or.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_smaller.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_smallerEqual.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_sub.h"

#include "../../inc/parsingAnalysis/ast/operators/unary/ast_decrement.h"
#include "../../inc/parsingAnalysis/ast/operators/unary/ast_increment.h"
#include "../../inc/parsingAnalysis/ast/operators/unary/ast_neg.h"
#include "../../inc/parsingAnalysis/ast/operators/unary/ast_not.h"

#include "../../inc/parsingAnalysis/ast/assignments/ast_assignment.h"
#include "../../inc/parsingAnalysis/ast/assignments/ast_selfAdd.h"
#include "../../inc/parsingAnalysis/ast/assignments/ast_selfDiv.h"
#include "../../inc/parsingAnalysis/ast/assignments/ast_selfMult.h"
#include "../../inc/parsingAnalysis/ast/assignments/ast_selfSub.h"

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

#include "../../inc/parsingAnalysis/ast/userTypes/ast_class.h"
#include "../../inc/parsingAnalysis/ast/userTypes/ast_enum.h"
#include "../../inc/parsingAnalysis/ast/userTypes/ast_struct.h"

#include "../../inc/parsingAnalysis/ast/variables/ast_autoDecl.h"
#include "../../inc/parsingAnalysis/ast/variables/ast_constDecl.h"
#include "../../inc/parsingAnalysis/ast/variables/ast_letDecl.h"
#include "../../inc/parsingAnalysis/ast/variables/ast_varCall.h"

#include "../../inc/parsingAnalysis/ast/chained/ast_chained.h"

#include "../../inc/parsingAnalysis/ast/tree.h"
#include <ostream>

namespace nicole {

std::expected<std::string, Error>
PrintTree::visit(const AST_BOOL *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  return indent_ + "Bool: " + (node->value() ? "true" : "false") + "\n";
}

std::expected<std::string, Error>
PrintTree::visit(const AST_CHAR *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  return indent_ + "Char: '" + node->value() + "'\n";
}

std::expected<std::string, Error>
PrintTree::visit(const AST_DOUBLE *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  return indent_ + "Double: " + std::to_string(node->value()) + "\n";
}

std::expected<std::string, Error>
PrintTree::visit(const AST_FLOAT *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  return indent_ + "Float: " + std::to_string(node->value()) + "\n";
}

std::expected<std::string, Error>
PrintTree::visit(const AST_INT *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  return indent_ + "Integer: " + std::to_string(node->value()) + "\n";
}

std::expected<std::string, Error>
PrintTree::visit(const AST_NULL *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  return indent_ + "null\n";
}

std::expected<std::string, Error>
PrintTree::visit(const AST_STRING *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  return indent_ + "String: \"" + node->value() + "\"\n";
}

std::expected<std::string, Error>
PrintTree::visit(const AST_VECTOR *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "vector:\n";
  increaseIndent();
  for (const auto &statement : node->values()) {
    const auto val{statement->accept(*this)};
    if (!val) {
    }
    result << *val;
  }
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_DELETE *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "delete:\n";
  increaseIndent();
  const auto val{node->value()->accept(*this)};
  if (!val) {
  }
  result << *val;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_NEW *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "new:\n";
  increaseIndent();
  const auto val{node->value()->accept(*this)};
  if (!val) {
  }
  result << *val;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_DEREF *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "deref:\n";
  increaseIndent();
  const auto val{node->value()->accept(*this)};
  if (!val) {
  }
  result << *val;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_PTR *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "ptr:\n";
  increaseIndent();
  result << indent_ << "id:\n" << node->id();
  result << indent_ << "type: " << node->valueType();
  const auto val{node->value()->accept(*this)};
  if (!val) {
  }
  result << indent_ << "value:\n" << *val;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_ADD *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "add:\n";
  increaseIndent();
  const auto left{node->left()->accept(*this)};
  if (!left) {
  }
  result << indent_ << "Left:\n" << *left;
  const auto right{node->right()->accept(*this)};
  if (!right) {
  }
  result << indent_ << "Right:\n" << *right;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_SUB *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "sub:\n";
  increaseIndent();
  const auto left{node->left()->accept(*this)};
  if (!left) {
  }
  result << indent_ << "Left:\n" << *left;
  const auto right{node->right()->accept(*this)};
  if (!right) {
  }
  result << indent_ << "Right:\n" << *right;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_MULT *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "mult:\n";
  increaseIndent();
  const auto left{node->left()->accept(*this)};
  if (!left) {
  }
  result << indent_ << "Left:\n" << *left;
  const auto right{node->right()->accept(*this)};
  if (!right) {
  }
  result << indent_ << "Right:\n" << *right;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_DIV *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "div:\n";
  increaseIndent();
  const auto left{node->left()->accept(*this)};
  if (!left) {
  }
  result << indent_ << "Left:\n" << *left;
  const auto right{node->right()->accept(*this)};
  if (!right) {
  }
  result << indent_ << "Right:\n" << *right;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_MODULE *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "module:\n";
  increaseIndent();
  const auto left{node->left()->accept(*this)};
  if (!left) {
  }
  result << indent_ << "Left:\n" << *left;
  const auto right{node->right()->accept(*this)};
  if (!right) {
  }
  result << indent_ << "Right:\n" << *right;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_SMALLER *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "smaller:\n";
  increaseIndent();
  const auto left{node->left()->accept(*this)};
  if (!left) {
  }
  result << indent_ << "Left:\n" << *left;
  const auto right{node->right()->accept(*this)};
  if (!right) {
  }
  result << indent_ << "Right:\n" << *right;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_SMALLER_EQUAL *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "smallerEqual:\n";
  increaseIndent();
  const auto left{node->left()->accept(*this)};
  if (!left) {
  }
  result << indent_ << "Left:\n" << *left;
  const auto right{node->right()->accept(*this)};
  if (!right) {
  }
  result << indent_ << "Right:\n" << *right;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_BIGGER *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "bigger:\n";
  increaseIndent();
  const auto left{node->left()->accept(*this)};
  if (!left) {
  }
  result << indent_ << "Left:\n" << *left;
  const auto right{node->right()->accept(*this)};
  if (!right) {
  }
  result << indent_ << "Right:\n" << *right;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_BIGGER_EQUAL *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "biggerEqual:\n";
  increaseIndent();
  const auto left{node->left()->accept(*this)};
  if (!left) {
  }
  result << indent_ << "Left:\n" << *left;
  const auto right{node->right()->accept(*this)};
  if (!right) {
  }
  result << indent_ << "Right:\n" << *right;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_EQUAL *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "equal:\n";
  increaseIndent();
  const auto left{node->left()->accept(*this)};
  if (!left) {
  }
  result << indent_ << "Left:\n" << *left;
  const auto right{node->right()->accept(*this)};
  if (!right) {
  }
  result << indent_ << "Right:\n" << *right;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_NOT_EQUAL *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "not equal:\n";
  increaseIndent();
  const auto left{node->left()->accept(*this)};
  if (!left) {
  }
  result << indent_ << "Left:\n" << *left;
  const auto right{node->right()->accept(*this)};
  if (!right) {
  }
  result << indent_ << "Right:\n" << *right;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_OR *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "or:\n";
  increaseIndent();
  const auto left{node->left()->accept(*this)};
  if (!left) {
  }
  result << indent_ << "Left:\n" << *left;
  const auto right{node->right()->accept(*this)};
  if (!right) {
  }
  result << indent_ << "Right:\n" << *right;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_AND *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "and:\n";
  increaseIndent();
  const auto left{node->left()->accept(*this)};
  if (!left) {
  }
  result << indent_ << "Left:\n" << *left;
  const auto right{node->right()->accept(*this)};
  if (!right) {
  }
  result << indent_ << "Right:\n" << *right;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_NEG *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "neg:\n";
  increaseIndent();
  const auto val{node->value()->accept(*this)};
  if (!val) {
  }
  result << indent_ << "value:\n" << *val;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_NOT *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "not:\n";
  increaseIndent();
  const auto val{node->value()->accept(*this)};
  if (!val) {
  }
  result << indent_ << "value:\n" << *val;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_DECREMENT *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "decrement:\n";
  increaseIndent();
  const auto val{node->value()->accept(*this)};
  if (!val) {
  }
  result << indent_ << "value:\n" << *val;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_INCREMENT *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "increment:\n";
  increaseIndent();
  const auto val{node->value()->accept(*this)};
  if (!val) {
  }
  result << indent_ << "value:\n" << *val;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_ASSIGNMENT *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "Variable Reassignment:\n";
  increaseIndent();
  result << indent_ << "Var: " << node->id() << "\n";
  const auto val{node->value()->accept(*this)};
  if (!val) {
  }
  result << indent_ << "Value:\n" << *val;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_SELF_ADD *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "Self add:\n";
  increaseIndent();
  result << indent_ << "Var: " << node->id() << "\n";
  const auto val{node->value()->accept(*this)};
  if (!val) {
  }
  result << indent_ << "Value:\n" << *val;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_SELF_SUB *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "Self sub:\n";
  increaseIndent();
  result << indent_ << "Var: " << node->id() << "\n";
  const auto val{node->value()->accept(*this)};
  if (!val) {
  }
  result << indent_ << "Value:\n" << *val;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_SELF_MULT *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "Self mult:\n";
  increaseIndent();
  result << indent_ << "Var: " << node->id() << "\n";
  const auto val{node->value()->accept(*this)};
  if (!val) {
  }
  result << indent_ << "Value:\n" << *val;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_SELF_DIV *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "Self Div:\n";
  increaseIndent();
  result << indent_ << "Var: " << node->id() << "\n";
  const auto val{node->value()->accept(*this)};
  if (!val) {
  }
  result << indent_ << "Value:\n" << *val;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_PRINT *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "Print Statement:\n";
  increaseIndent();
  increaseIndent();
  for (const auto &expr : node->values()) {
    const auto toStr{expr->accept(*this)};
    if (!toStr) {
    }
    result << indent_ << "Expression:\n" << *toStr;
  }
  decreaseIndent();
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_IMPORT *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "Import:\n";
  increaseIndent();
  result << indent_ << "File: " << node->path().string() << "\n";
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_STATEMENT *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "statement:\n";
  increaseIndent();
  const auto expr{node->expression()->accept(*this)};
  if (!expr) {
  }
  result << *expr;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_BODY *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "Statement List:\n";
  increaseIndent();
  for (const auto &statement : node->body()) {
    const auto val{statement->accept(*this)};
    result << *val;
  }
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_COMMA *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "Comma List:\n";
  increaseIndent();
  for (const auto &statement : node->body()) {
    const auto val{statement->accept(*this)};
    result << *val;
  }
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_WHILE *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "While:\n";
  increaseIndent();
  result << "Condition: ";
  const auto condition{node->condition()->accept(*this)};
  if (!condition) {
  }
  result << *condition;
  result << "Body:\n";
  for (const auto &statement : node->body()->body()) {
    const auto val{statement->accept(*this)};
    if (!val) {
    }
    result << *val;
  }
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_FOR *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "For:\n";
  increaseIndent();
  result << "Init: ";
  const auto init{node->init()->accept(*this)};
  if (!init) {
  }
  result << *init;
  result << "Condition: ";
  const auto condition{node->condition()->accept(*this)};
  if (!condition) {
  }
  result << *condition;
  result << "Update: ";
  const auto update{node->update()->accept(*this)};
  if (!update) {
  }
  result << *update;
  result << "Body:\n";
  for (const auto &statement : node->body()->body()) {
    const auto val{statement->accept(*this)};
    if (!val) {
    }
    result << *val;
  }
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_DO_WHILE *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "Do While:\n";
  increaseIndent();
  result << "Body:\n";
  for (const auto &statement : node->body()->body()) {
    const auto val{statement->accept(*this)};
    if (!val) {
    }
    result << *val;
  }
  result << "Condition: ";
  const auto condition{node->condition()->accept(*this)};
  if (!condition) {
  }
  result << *condition;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_PASS *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  increaseIndent();
  result << indent_ << "Pass Statement:\n";
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_STOP *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  increaseIndent();
  result << indent_ << "Stop Statement:\n";
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_IF *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "print if"}};
}

std::expected<std::string, Error>
PrintTree::visit(const AST_ELSE_IF *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "print else if"}};
}

std::expected<std::string, Error>
PrintTree::visit(const AST_SWITCH *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "Switch Statement:\n";
  increaseIndent();
  result << indent_ << "Expression:\n";
  const auto expr{node->condition()->accept(*this)};
  if (!expr) {
  }
  result << *expr;

  result << indent_ << "Cases:\n";
  for (const auto &caseStmt : node->cases()) {
    const auto caseStr{caseStmt->accept(*this)};
    if (!caseStr) {
      return std::unexpected{caseStr.error()};
    }
    result << *caseStr;
  }
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_CASE *node) const noexcept {
  if (!node) {
  }
  std::ostringstream result;
  result << indent_ << "Case:\n";
  increaseIndent();
  result << indent_ << "Condition:\n";
  const auto cond{node->match()->accept(*this)};
  if (!cond) {
    return std::unexpected{cond.error()};
  }
  result << *cond;

  result << indent_ << "Body:\n";
  for (const auto &statement : node->body()->body()) {
    const auto stmt{statement->accept(*this)};
    if (!stmt) {
      return std::unexpected{stmt.error()};
    }
    result << *stmt;
  }
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_DEFAULT *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "Default Case:\n";
  increaseIndent();
  for (const auto &statement : node->body()->body()) {
    const auto stmt{statement->accept(*this)};
    if (!stmt) {
      return std::unexpected{stmt.error()};
    }
    result << *stmt;
  }
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_TERNARY *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "Ternary Expression:\n";
  increaseIndent();
  result << indent_ << "Condition:\n";
  const auto cond{node->condition()->accept(*this)};
  if (!cond) {
    return std::unexpected{cond.error()};
  }
  result << *cond;

  result << indent_ << "True Branch:\n";
  const auto trueBranch{node->first()->accept(*this)};
  if (!trueBranch) {
    return std::unexpected{trueBranch.error()};
  }
  result << *trueBranch;

  result << indent_ << "False Branch:\n";
  const auto falseBranch{node->second()->accept(*this)};
  if (!falseBranch) {
    return std::unexpected{falseBranch.error()};
  }
  result << *falseBranch;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_FUNC_CALL *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "print func call"}};
}

std::expected<std::string, Error>
PrintTree::visit(const AST_FUNC_DECL *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "print func decl"}};
}

std::expected<std::string, Error>
PrintTree::visit(const AST_RETURN *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "Return:\n";
  increaseIndent();
  result << "value: " << node->expression();
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_ENUM *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "Enum:\n";
  increaseIndent();
  result << "id: " << node->id();
  result << "names:\n";
  for (const auto &id : node->identifiers()) {
    result << id << " ";
  }
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_STRUCT *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "print struct"}};
}

std::expected<std::string, Error>
PrintTree::visit(const AST_CLASS *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "print class"}};
}

std::expected<std::string, Error>
PrintTree::visit(const AST_AUTO_DECL *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << "auto decl\n";
  increaseIndent();
  result << "id: " << node->id();
  const auto val{node->value()->accept(*this)};
  if (!val) {
  }
  result << *val;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_LET_DECL *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << "let decl\n";
  increaseIndent();
  result << "id: " << node->id();
  result << "type: " << node->valueType();
  const auto val{node->value()->accept(*this)};
  if (!val) {
  }
  result << *val;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_CONST_DECL *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << "const decl\n";
  increaseIndent();
  result << "id: " << node->id();
  result << "type: " << node->valueType();
  const auto val{node->value()->accept(*this)};
  if (!val) {
  }
  result << *val;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_VAR_CALL *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << "Var call\n";
  increaseIndent();
  result << "id: " << node->id();
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_CHAINED *node) const noexcept {
  if (!node) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << indent_ << "Chained:\n";
  increaseIndent();
  result << "Base: " << node->base();
  result << "Operations:\n";
  for (const auto &statement : node->operations()) {
    const auto val{statement->accept(*this)};
    result << *val;
  }
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const Tree *tree) const noexcept {
  if (!tree) {
    // std::unexpected{Error{, ""}};
  }
  std::ostringstream result;
  result << "Tree:\n";
  increaseIndent();
  const auto str{tree->root()->accept(*this)};
  if (!str) {
    return std::unexpected{str.error()};
  }
  result << *str;
  decreaseIndent();
  return result.str();
}

} // namespace nicole
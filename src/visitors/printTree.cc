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
#include "../../inc/parsingAnalysis/ast/pointer/ast_new.h"
#include "../../inc/parsingAnalysis/ast/pointer/ast_deref.h"
#include "../../inc/parsingAnalysis/ast/pointer/ast_ptr.h"

#include "../../inc/parsingAnalysis/ast/operators/binary/ast_add.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_sub.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_mult.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_div.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_module.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_smaller.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_smallerEqual.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_bigger.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_biggerEqual.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_equal.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_notEqual.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_or.h"
#include "../../inc/parsingAnalysis/ast/operators/binary/ast_and.h"

#include "../../inc/parsingAnalysis/ast/operators/unary/ast_decrement.h"
#include "../../inc/parsingAnalysis/ast/operators/unary/ast_increment.h"
#include "../../inc/parsingAnalysis/ast/operators/unary/ast_neg.h"
#include "../../inc/parsingAnalysis/ast/operators/unary/ast_not.h"

#include "../../inc/parsingAnalysis/ast/assignments/ast_assignment.h"
#include "../../inc/parsingAnalysis/ast/assignments/ast_selfAdd.h"
#include "../../inc/parsingAnalysis/ast/assignments/ast_selfSub.h"
#include "../../inc/parsingAnalysis/ast/assignments/ast_selfMult.h"
#include "../../inc/parsingAnalysis/ast/assignments/ast_selfDiv.h"

#include "../../inc/parsingAnalysis/ast/utils/ast_import.h"
#include "../../inc/parsingAnalysis/ast/utils/ast_print.h"

#include "../../inc/parsingAnalysis/ast/loops/ast_doWhile.h"
#include "../../inc/parsingAnalysis/ast/loops/ast_while.h"
#include "../../inc/parsingAnalysis/ast/loops/ast_for.h"
#include "../../inc/parsingAnalysis/ast/loops/ast_pass.h"
#include "../../inc/parsingAnalysis/ast/loops/ast_stop.h"

#include "../../inc/parsingAnalysis/ast/conditionals/ast_case.h"
#include "../../inc/parsingAnalysis/ast/conditionals/ast_default.h"
#include "../../inc/parsingAnalysis/ast/conditionals/ast_switch.h"
#include "../../inc/parsingAnalysis/ast/conditionals/ast_if.h"
#include "../../inc/parsingAnalysis/ast/conditionals/ast_elseIf.h"
#include "../../inc/parsingAnalysis/ast/conditionals/ast_ternary.h"

#include "../../inc/parsingAnalysis/ast/functions/ast_funcDecl.h"
#include "../../inc/parsingAnalysis/ast/functions/ast_funcCall.h"
#include "../../inc/parsingAnalysis/ast/functions/ast_return.h"

#include "../../inc/parsingAnalysis/ast/userTypes/ast_class.h"
#include "../../inc/parsingAnalysis/ast/userTypes/ast_struct.h"
#include "../../inc/parsingAnalysis/ast/userTypes/ast_enum.h"

#include "../../inc/parsingAnalysis/ast/variables/ast_autoDecl.h"
#include "../../inc/parsingAnalysis/ast/variables/ast_letDecl.h"
#include "../../inc/parsingAnalysis/ast/variables/ast_constDecl.h"
#include "../../inc/parsingAnalysis/ast/variables/ast_varCall.h"

#include "../../inc/parsingAnalysis/ast/chained/ast_chained.h"

#include "../../inc/parsingAnalysis/ast/tree.h"

namespace nicole {

std::expected<std::string, Error>
PrintTree::visit(const AST_BOOL *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_CHAR *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_DOUBLE *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_FLOAT *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_INT *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_NULL *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_STRING *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_VECTOR *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_DELETE *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_NEW *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_DEREF *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_PTR *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_ADD *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_SUB *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_MULT *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_DIV *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_MODULE *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_SMALLER *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_SMALLER_EQUAL *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_BIGGER *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_BIGGER_EQUAL *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_EQUAL *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_NOT_EQUAL *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_OR *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_AND *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_NEG *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_NOT *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_DECREMENT *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_INCREMENT *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_ASSIGNMENT *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_SELF_ADD *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_SELF_SUB *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_SELF_MULT *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_SELF_DIV *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_PRINT *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_IMPORT *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_STATEMENT *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_BODY *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_COMMA *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_WHILE *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_FOR *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_DO_WHILE *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_PASS *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_STOP *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_IF *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_ELSE_IF *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_SWITCH *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_CASE *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_DEFAULT *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_TERNARY *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_FUNC_CALL *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_FUNC_DECL *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_RETURN *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_ENUM *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_STRUCT *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_CLASS *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_AUTO_DECL *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_LET_DECL *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_CONST_DECL *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_VAR_CALL *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const AST_CHAINED *node) const noexcept {
  return astTypeToStr(node->type());
}

std::expected<std::string, Error>
PrintTree::visit(const Tree *tree) const noexcept {
  return tree->root()->accept(*this);
}

} // namespace nicole
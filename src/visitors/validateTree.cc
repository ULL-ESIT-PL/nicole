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

#include "../../inc/parsingAnalysis/ast/userTypes/ast_attrAccess.h"
#include "../../inc/parsingAnalysis/ast/userTypes/ast_class.h"
#include "../../inc/parsingAnalysis/ast/userTypes/ast_constructorCall.h"
#include "../../inc/parsingAnalysis/ast/userTypes/ast_enum.h"
#include "../../inc/parsingAnalysis/ast/userTypes/ast_methodCall.h"
#include "../../inc/parsingAnalysis/ast/userTypes/ast_struct.h"
#include "../../inc/parsingAnalysis/ast/userTypes/ast_this.h"

#include "../../inc/parsingAnalysis/ast/variables/ast_autoDecl.h"
#include "../../inc/parsingAnalysis/ast/variables/ast_constDecl.h"
#include "../../inc/parsingAnalysis/ast/variables/ast_letDecl.h"
#include "../../inc/parsingAnalysis/ast/variables/ast_varCall.h"

#include "../../inc/parsingAnalysis/ast/chained/ast_chained.h"

#include "../../inc/parsingAnalysis/ast/tree.h"
#include <iostream>

namespace nicole {

std::expected<bool, Error>
ValidateTree::visit(const AST_BOOL *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_BOOL"}};
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_CHAR *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_CHAR"}};
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_DOUBLE *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_DOUBLE"}};
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_FLOAT *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_FLOAT"}};
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_INT *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_INT"}};
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_NULL *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_NULL"}};
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_STRING *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_STRING"}};
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_VECTOR *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_VECTOR"}};
  }
  return true;
}

// chained
std::expected<bool, Error>
ValidateTree::visit(const AST_INDEX *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_INDEX"}};
  }
  return true;
}

// statemetn / body / not null
std::expected<bool, Error>
ValidateTree::visit(const AST_DELETE *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_DELETE"}};
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_NEW *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_NEW"}};
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_DEREF *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_DEREF"}};
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_PTR *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_PTR"}};
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_ADD *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_ADD"}};
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_SUB *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_SUB"}};
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_MULT *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_MULT"}};
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_DIV *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_DIV"}};
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_MODULE *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_MODULE"}};
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_SMALLER *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_SMALLER"}};
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_SMALLER_EQUAL *node) const noexcept {
  if (!node) {
    return std::unexpected{
        Error{ERROR_TYPE::NULL_NODE, "invalid AST_SMALLER_EQUAL"}};
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_BIGGER *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_BIGGER"}};
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_BIGGER_EQUAL *node) const noexcept {
  if (!node) {
    return std::unexpected{
        Error{ERROR_TYPE::NULL_NODE, "invalid AST_BIGGER_EQUAL"}};
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_EQUAL *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_EQUAL"}};
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_NOT_EQUAL *node) const noexcept {
  if (!node) {
    return std::unexpected{
        Error{ERROR_TYPE::NULL_NODE, "invalid AST_NOT_EQUAL"}};
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_OR *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_OR"}};
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_AND *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_AND"}};
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_NEG *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_NEG"}};
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_NOT *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_NOT"}};
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_DECREMENT *node) const noexcept {
  if (!node) {
    return std::unexpected{
        Error{ERROR_TYPE::NULL_NODE, "invalid AST_DECREMENT"}};
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_INCREMENT *node) const noexcept {
  if (!node) {
    return std::unexpected{
        Error{ERROR_TYPE::NULL_NODE, "invalid AST_INCREMENT"}};
  }
  return true;
}

// statemetn / body / not null or for
std::expected<bool, Error>
ValidateTree::visit(const AST_ASSIGNMENT *node) const noexcept {
  if (!node) {
    return std::unexpected{
        Error{ERROR_TYPE::NULL_NODE, "invalid AST_ASSIGNMENT"}};
  }
  return true;
}

// statemetn / body / not null or for
std::expected<bool, Error>
ValidateTree::visit(const AST_SELF_ADD *node) const noexcept {
  if (!node) {
    return std::unexpected{
        Error{ERROR_TYPE::NULL_NODE, "invalid AST_SELF_ADD"}};
  }
  return true;
}

// statemetn / body / not null or for
std::expected<bool, Error>
ValidateTree::visit(const AST_SELF_SUB *node) const noexcept {
  if (!node) {
    return std::unexpected{
        Error{ERROR_TYPE::NULL_NODE, "invalid AST_SELF_SUB"}};
  }
  return true;
}

// statemetn / body / not null or for
std::expected<bool, Error>
ValidateTree::visit(const AST_SELF_MULT *node) const noexcept {
  if (!node) {
    return std::unexpected{
        Error{ERROR_TYPE::NULL_NODE, "invalid AST_SELF_MULT"}};
  }
  return true;
}

// statemetn / body / not null or for
std::expected<bool, Error>
ValidateTree::visit(const AST_SELF_DIV *node) const noexcept {
  if (!node) {
    return std::unexpected{
        Error{ERROR_TYPE::NULL_NODE, "invalid AST_SELF_DIV"}};
  }
  return true;
}

// statement / body / not null
std::expected<bool, Error>
ValidateTree::visit(const AST_PRINT *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_PRINT"}};
  }
  return true;
}

// statement / body / null
std::expected<bool, Error>
ValidateTree::visit(const AST_IMPORT *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_IMPORT"}};
  }
  return true;
}

// not relevant
std::expected<bool, Error>
ValidateTree::visit(const AST_STATEMENT *node) const noexcept {
  if (!node) {
    return std::unexpected{
        Error{ERROR_TYPE::NULL_NODE, "invalid AST_STATEMENT"}};
  }
  return true;
}

// not relevant
std::expected<bool, Error>
ValidateTree::visit(const AST_BODY *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_BODY"}};
  }
  return true;
}

// not relevant
std::expected<bool, Error>
ValidateTree::visit(const AST_COMMA *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_COMMA"}};
  }
  return true;
}

// statement / body / not null
std::expected<bool, Error>
ValidateTree::visit(const AST_WHILE *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_WHILE"}};
  }
  return true;
}

// statement / body / not null
std::expected<bool, Error>
ValidateTree::visit(const AST_FOR *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_FOR"}};
  }
  return true;
}

// statement / body / not null
std::expected<bool, Error>
ValidateTree::visit(const AST_DO_WHILE *node) const noexcept {
  if (!node) {
    return std::unexpected{
        Error{ERROR_TYPE::NULL_NODE, "invalid AST_DO_WHILE"}};
  }
  return true;
}

// do while / while / for
std::expected<bool, Error>
ValidateTree::visit(const AST_PASS *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_PASS"}};
  }
  return true;
}

// do while / while / for
std::expected<bool, Error>
ValidateTree::visit(const AST_STOP *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_STOP"}};
  }
  return true;
}

// statement / body / not null
std::expected<bool, Error>
ValidateTree::visit(const AST_IF *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_IF"}};
  }
  return true;
}

// if
std::expected<bool, Error>
ValidateTree::visit(const AST_ELSE_IF *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_ELSE_IF"}};
  }
  return true;
}

// statement / body / not null
std::expected<bool, Error>
ValidateTree::visit(const AST_SWITCH *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_SWITCH"}};
  }
  return true;
}

// switch
std::expected<bool, Error>
ValidateTree::visit(const AST_CASE *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_CASE"}};
  }
  return true;
}

// switch
std::expected<bool, Error>
ValidateTree::visit(const AST_DEFAULT *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_DEFAULT"}};
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_TERNARY *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_TERNARY"}};
  }
  return true;
}

// chained
std::expected<bool, Error>
ValidateTree::visit(const AST_FUNC_CALL *node) const noexcept {
  if (!node) {
    return std::unexpected{
        Error{ERROR_TYPE::NULL_NODE, "Invalid AST_FUNC_CALL"}};
  }
  return true;
}

// statement / body / null or struct / class
std::expected<bool, Error>
ValidateTree::visit(const AST_FUNC_DECL *node) const noexcept {
  if (!node) {
    return std::unexpected{
        Error{ERROR_TYPE::NULL_NODE, "invalid AST_FUNC_DECL"}};
  }
  return true;
}

// func decl
std::expected<bool, Error>
ValidateTree::visit(const AST_RETURN *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_RETURN"}};
  }
  return true;
}

// statement / body / null
std::expected<bool, Error>
ValidateTree::visit(const AST_ENUM *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_ENUM"}};
  }
  return true;
}

// statement / body / null
std::expected<bool, Error>
ValidateTree::visit(const AST_STRUCT *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_STRUCT"}};
  }
  return true;
}

// statement / body / null
std::expected<bool, Error>
ValidateTree::visit(const AST_CLASS *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_CLASS"}};
  }
  return true;
}

// chained
std::expected<bool, Error>
ValidateTree::visit(const AST_ATTR_ACCESS *node) const noexcept {
  if (!node) {
    return std::unexpected{
        Error{ERROR_TYPE::NULL_NODE, "invalid AST_ATTR_ACCESS"}};
  }
  return true;
}

// chained
std::expected<bool, Error>
ValidateTree::visit(const AST_METHOD_CALL *node) const noexcept {
  if (!node) {
    return std::unexpected{
        Error{ERROR_TYPE::NULL_NODE, "Invalid AST_METHOD_CALL"}};
  }
  return true;
}

// func decl / struct
std::expected<bool, Error>
ValidateTree::visit(const AST_THIS *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_THIS"}};
  }
  return true;
}

// chained
std::expected<bool, Error>
ValidateTree::visit(const AST_CONSTRUCTOR_CALL *node) const noexcept {
  if (!node) {
    return std::unexpected{
        Error{ERROR_TYPE::NULL_NODE, "Invalid AST_CONSTRUCTOR_CALL"}};
  }
  return true;
}

// statement / body / not null
std::expected<bool, Error>
ValidateTree::visit(const AST_AUTO_DECL *node) const noexcept {
  if (!node) {
    return std::unexpected{
        Error{ERROR_TYPE::NULL_NODE, "invalid AST_AUTO_DECL"}};
  }
  return true;
}

// statement / body / not null
std::expected<bool, Error>
ValidateTree::visit(const AST_LET_DECL *node) const noexcept {
  if (!node) {
    return std::unexpected{
        Error{ERROR_TYPE::NULL_NODE, "invalid AST_LET_DECL"}};
  }
  return true;
}

// statement / body / not null
std::expected<bool, Error>
ValidateTree::visit(const AST_CONST_DECL *node) const noexcept {
  if (!node) {
    return std::unexpected{
        Error{ERROR_TYPE::NULL_NODE, "invalid AST_CONST_DECL"}};
  }
  return true;
}

// chained
std::expected<bool, Error>
ValidateTree::visit(const AST_VAR_CALL *node) const noexcept {
  if (!node) {
    return std::unexpected{
        Error{ERROR_TYPE::NULL_NODE, "invalid AST_VAR_CALL"}};
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const AST_CHAINED *node) const noexcept {
  if (!node) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid AST_CHAINED"}};
  }
  return true;
}

std::expected<bool, Error>
ValidateTree::visit(const Tree *tree) const noexcept {
  if (!tree) {
    return std::unexpected{Error{ERROR_TYPE::NULL_NODE, "invalid Tree"}};
  }
  return tree->root()->accept(*this);
}

} // namespace nicole
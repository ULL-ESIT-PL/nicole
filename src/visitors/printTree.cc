#include "../../inc/visitors/printTree.h"
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
#include "../../inc/parsingAnalysis/ast/userTypes/ast_class.h"
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
#include <ostream>

namespace nicole {

std::expected<std::string, Error>
PrintTree::visit(const AST_BOOL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_BOOL");
  }
  return indent_ + "Bool: " + (node->value() ? "true" : "false") + "\n";
}

std::expected<std::string, Error>
PrintTree::visit(const AST_CHAR *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_CHAR");
  }
  return indent_ + "Char: '" + node->value() + "'\n";
}

std::expected<std::string, Error>
PrintTree::visit(const AST_DOUBLE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DOUBLE");
  }
  return indent_ + "Double: " + std::to_string(node->value()) + "\n";
}

std::expected<std::string, Error>
PrintTree::visit(const AST_FLOAT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_FLOAT");
  }
  return indent_ + "Float: " + std::to_string(node->value()) + "\n";
}

std::expected<std::string, Error>
PrintTree::visit(const AST_INT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_INT");
  }
  return indent_ + "Integer: " + std::to_string(node->value()) + "\n";
}

std::expected<std::string, Error>
PrintTree::visit(const AST_NULL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_NULL");
  }
  return indent_ + "null\n";
}

std::expected<std::string, Error>
PrintTree::visit(const AST_STRING *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_STRING");
  }
  return indent_ + "String: \"" + node->value() + "\"\n";
}

std::expected<std::string, Error>
PrintTree::visit(const AST_VECTOR *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_VECTOR");
  }
  std::ostringstream result;
  result << indent_ << "vector:\n";
  increaseIndent();
  for (const auto &statement : node->values()) {
    const auto val{statement->accept(*this)};
    if (!val) {
      return createError(val.error());
    }
    result << *val;
  }
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_INDEX *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_INDEX");
  }
  std::ostringstream result;
  result << indent_ << "index:\n";
  increaseIndent();
  const auto val{node->index()->accept(*this)};
  if (!val) {
    return createError(val.error());
  }
  result << *val;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_DELETE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DELETE");
  }
  std::ostringstream result;
  result << indent_ << "delete:\n";
  increaseIndent();
  const auto val{node->value()->accept(*this)};
  if (!val) {
    return createError(val.error());
  }
  result << *val;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_NEW *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_NEW");
  }
  std::ostringstream result;
  result << indent_ << "new:\n";
  increaseIndent();
  const auto val{node->value()->accept(*this)};
  if (!val) {
    return createError(val.error());
  }
  result << *val;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_DEREF *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DEREF");
  }
  std::ostringstream result;
  result << indent_ << "deref:\n";
  increaseIndent();
  const auto val{node->value()->accept(*this)};
  if (!val) {
    return createError(val.error());
  }
  result << *val;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_PTR *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_PTR");
  }
  std::ostringstream result;
  result << indent_ << "ptr:\n";
  increaseIndent();
  result << indent_ << "id:\n" << node->id();
  result << indent_ << "type: " << node->valueType();
  const auto val{node->value()->accept(*this)};
  if (!val) {
    return createError(val.error());
  }
  result << indent_ << "value:\n" << *val;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_BINARY *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_BINARY");
  }
  std::ostringstream result;
  result << indent_ << node->op().raw() + ":\n";
  increaseIndent();
  const auto left{node->left()->accept(*this)};
  if (!left) {
    return createError(left.error());
  }
  result << indent_ << "Left:\n" << *left;
  const auto right{node->right()->accept(*this)};
  if (!right) {
    return createError(right.error());
  }
  result << indent_ << "Right:\n" << *right;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_UNARY *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_UNARY");
  }
  std::ostringstream result;
  result << indent_ << node->op().raw() + ":\n";
  increaseIndent();
  const auto val{node->value()->accept(*this)};
  if (!val) {
    return createError(val.error());
  }
  result << indent_ << "value:\n" << *val;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_ASSIGNMENT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ASSIGNMENT");
  }
  std::ostringstream result;
  result << indent_ << "Assignment:\n";
  increaseIndent();
  result << indent_ << "Operator: " << node->op().raw() << "\n";
  const auto left{node->left()->accept(*this)};
  if (!left) {
    return createError(left.error());
  }
  result << indent_ << "left:\n" << *left << "\n";
  const auto val{node->value()->accept(*this)};
  if (!val) {
    return createError(val.error());
  }
  result << indent_ << "Value:\n" << *val << "\n";
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_PRINT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_PRINT");
  }
  std::ostringstream result;
  result << indent_ << "Print Statement:\n";
  increaseIndent();
  increaseIndent();
  for (const auto &expr : node->values()) {
    const auto toStr{expr->accept(*this)};
    if (!toStr) {
      return createError(toStr.error());
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
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_IMPORT");
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
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_STATEMENT");
  }
  std::ostringstream result;
  result << indent_ << "statement:\n";
  increaseIndent();
  const auto expr{node->expression()->accept(*this)};
  if (!expr) {
    return createError(expr.error());
  }
  result << *expr;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_BODY *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_BODY");
  }
  std::ostringstream result;
  result << indent_ << "Body:\n";
  increaseIndent();
  for (const auto &statement : node->body()) {
    const auto val{statement->accept(*this)};
    if (!val) {
      return createError(val.error());
    }
    result << *val;
  }
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_WHILE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_WHILE");
  }
  std::ostringstream result;
  result << indent_ << "While Loop:\n";
  increaseIndent();

  increaseIndent();
  const auto condition{node->condition()->accept(*this)};
  if (!condition) {
    return createError(condition.error());
  }
  result << *condition;
  decreaseIndent();

  for (const auto &statement : node->body()->body()) {
    const auto val{statement->accept(*this)};
    if (!val) {
      return createError(val.error());
    }
    result << *val;
  }

  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_FOR *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_FOR");
  }
  std::ostringstream result;
  result << indent_ << "For Loop:\n";
  increaseIndent();

  result << indent_ << "Init:\n";
  increaseIndent();
  for (const auto &initExpr : node->init()) {
    const auto initStr{initExpr->accept(*this)};
    if (!initStr) {
      return createError(initStr.error());
    }
    result << *initStr;
  }
  decreaseIndent();

  const auto condition{node->condition()->accept(*this)};
  if (!condition) {
    return createError(condition.error());
  }
  result << *condition;

  result << indent_ << "Update:\n";
  increaseIndent();
  for (const auto &updateExpr : node->update()) {
    const auto updateStr{updateExpr->accept(*this)};
    if (!updateStr) {
      return createError(updateStr.error());
    }
    result << *updateStr;
  }
  decreaseIndent();

  for (const auto &statement : node->body()->body()) {
    const auto val{statement->accept(*this)};
    if (!val) {
      return createError(val.error());
    }
    result << *val;
  }

  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_DO_WHILE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DO_WHILE");
  }
  std::ostringstream result;
  result << indent_ << "Do While Loop:\n";
  increaseIndent();

  for (const auto &statement : node->body()->body()) {
    const auto val{statement->accept(*this)};
    if (!val) {
      return createError(val.error());
    }
    result << *val;
  }
  const auto condition{node->condition()->accept(*this)};
  if (!condition) {
    return createError(condition.error());
  }
  result << *condition;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_PASS *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_PASS");
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
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_STOP");
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
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_IF");
  }
  std::ostringstream result;
  result << indent_ << "If Statement:\n";
  increaseIndent();
  const auto condition{node->condition()->accept(*this)};
  if (!condition) {
    return createError(condition.error());
  }
  result << *condition;

  for (const auto &statement : node->body()->body()) {
    const auto stmt{statement->accept(*this)};
    if (!stmt) {
      return createError(stmt.error());
    }
    result << *stmt;
  }

  if (node->elseIf().size() > 0) {
    result << indent_ << "Else Ifs:\n";
    for (const auto &elseIf : node->elseIf()) {
      const auto elseIfStr{elseIf->accept(*this)};
      if (!elseIfStr) {
        return createError(elseIfStr.error());
      }
      result << *elseIfStr;
    }
  }

  if (node->elseBody()) {
    result << indent_ << "Else:\n";
    for (const auto &statement : node->elseBody()->body()) {
      const auto stmt{statement->accept(*this)};
      if (!stmt) {
        return createError(stmt.error());
      }
      result << *stmt;
    }
  }

  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_ELSE_IF *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ELSE_IF");
  }
  std::ostringstream result;
  result << indent_ << "Else If:\n";
  increaseIndent();
  const auto condition{node->condition()->accept(*this)};
  if (!condition) {
    return createError(condition.error());
  }
  result << *condition;

  for (const auto &statement : node->body()->body()) {
    const auto stmt{statement->accept(*this)};
    if (!stmt) {
      return createError(stmt.error());
    }
    result << *stmt;
  }

  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_SWITCH *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_SWITCH");
  }
  std::ostringstream result;
  result << indent_ << "Switch Statement:\n";
  increaseIndent();
  const auto expr{node->condition()->accept(*this)};
  if (!expr) {
    return createError(expr.error());
  }
  result << *expr;

  result << indent_ << "Cases:\n";
  for (const auto &caseStmt : node->cases()) {
    const auto caseStr{caseStmt->accept(*this)};
    if (!caseStr) {
      return createError(caseStr.error());
    }
    result << *caseStr;
  }
  if (node->defaultCase()) {
    const auto defaultCase{node->defaultCase()->accept(*this)};
    if (!defaultCase) {
      return createError(defaultCase.error());
    }
    result << *defaultCase;
  }
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_CASE *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_CASE");
  }
  std::ostringstream result;
  result << indent_ << "Case:\n";
  increaseIndent();
  const auto cond{node->match()->accept(*this)};
  if (!cond) {
    return createError(cond.error());
  }
  result << *cond;

  for (const auto &statement : node->body()->body()) {
    const auto stmt{statement->accept(*this)};
    if (!stmt) {
      return createError(stmt.error());
    }
    result << *stmt;
  }
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_DEFAULT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_DEFAULT");
  }
  std::ostringstream result;
  result << indent_ << "Default Case:\n";
  increaseIndent();
  for (const auto &statement : node->body()->body()) {
    const auto stmt{statement->accept(*this)};
    if (!stmt) {
      return createError(stmt.error());
    }
    result << *stmt;
  }
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_TERNARY *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_TERNARY");
  }
  std::ostringstream result;
  result << indent_ << "Ternary Expression:\n";
  increaseIndent();
  const auto cond{node->condition()->accept(*this)};
  if (!cond) {
    return createError(cond.error());
  }
  result << *cond;

  result << indent_ << "True Branch:\n";
  const auto trueBranch{node->first()->accept(*this)};
  if (!trueBranch) {
    return createError(trueBranch.error());
  }
  result << *trueBranch;

  result << indent_ << "False Branch:\n";
  const auto falseBranch{node->second()->accept(*this)};
  if (!falseBranch) {
    return createError(falseBranch.error());
  }
  result << *falseBranch;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_CONDITION *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_CONDITION");
  }
  std::ostringstream result;
  result << indent_ << "Condition:\n";
  increaseIndent();
  const auto expr{node->condition()->accept(*this)};
  if (!expr) {
    return createError(expr.error());
  }
  result << *expr;
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_FUNC_CALL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_FUNC_CALL");
  }

  std::ostringstream result;
  result << indent_ << "Function Call:\n";
  increaseIndent();
  result << indent_ << "Name: " << node->id() << "\n";
  result << indent_ << "Arguments:\n";
  increaseIndent();
  for (const auto &arg : node->parameters()) {
    const auto argStr = arg->accept(*this);
    if (!argStr) {
      return createError(argStr.error());
    }
    result << *argStr;
  }
  decreaseIndent();
  decreaseIndent();

  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_FUNC_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_FUNC_DECL");
  }
  std::ostringstream result;
  result << indent_ << "Function Declaration:\n";
  increaseIndent();
  result << indent_ << "Name: " << node->id() << "\n";
  result << indent_ << "Generics: ";
  for (const auto &generic : node->generics()) {
    result << generic.name() << ", ";
  }
  result << "\n";
  result << indent_ << "Return Type: " << node->returnType() << "\n";

  result << indent_ << "Parameters:\n";
  increaseIndent();
  for (const auto &param : node->parameters()) {
    result << indent_ << "var " << param.first << " type " << param.second
           << "\n";
  }
  decreaseIndent();

  if (node->body()) {
    const auto bodyStr{node->body()->accept(*this)};
    if (!bodyStr) {
      return createError(bodyStr.error());
    }
    result << *bodyStr;
  } else {
    result << indent_ << "(empty body)\n";
  }
  decreaseIndent();

  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_RETURN *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_RETURN");
  }
  std::ostringstream result;
  result << indent_ << "Return:\n";
  increaseIndent();
  if (node->expression()) {
    const auto val{node->expression()->accept(*this)};
    if (!val) {
      return createError(val.error());
    }
    result << indent_ << "value:\n" << *val;
  } else {
    result << indent_ << "empty";
  }
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_ENUM *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ENUM");
  }
  std::ostringstream result;
  result << indent_ << "Enum:\n";
  increaseIndent();
  result << indent_ << "id: " << node->id() << "\n";
  result << indent_ << "names:\n";
  for (const auto &id : node->identifiers()) {
    result << indent_ << id << "\n";
  }
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_STRUCT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_STRUCT");
  }

  std::ostringstream result;
  result << indent_ << "Struct Declaration:\n";
  increaseIndent();

  // Nombre de la estructura
  result << indent_ << "Name: " << node->id() << "\n";
  result << indent_ << "Generics: ";
  for (const auto &generic : node->generics()) {
    result << generic.name() << ", ";
  }
  result << "\n";
  // Tipo padre (si existe)
  if (node->fatherType()) {
    result << indent_ << "Father Type: " << *node->fatherType() << "\n";
  } else {
    result << indent_ << "No Father Type\n";
  }

  // Atributos
  result << indent_ << "Attributes:\n";
  increaseIndent();
  for (const auto &[type, name] : node->attributes().params()) {
    result << indent_ << type << " " << name << "\n";
  }
  decreaseIndent();

  // Métodos
  result << indent_ << "Methods:\n";
  increaseIndent();
  for (const auto &method : node->methods()) {
    const auto methodStr = method->accept(*this);
    if (!methodStr) {
      return createError(methodStr.error());
    }
    result << *methodStr;
  }
  decreaseIndent();

  // Constructor
  if (node->constructor()) {
    increaseIndent();
    const auto ctorStr = node->constructor()->accept(*this);
    if (!ctorStr) {
      return createError(ctorStr.error());
    }
    result << *ctorStr;
    decreaseIndent();
  }

  // Destructor
  if (node->destructor()) {
    increaseIndent();
    const auto dtorStr = node->destructor()->accept(*this);
    if (!dtorStr) {
      return createError(dtorStr.error());
    }
    result << *dtorStr;
    decreaseIndent();
  }

  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_CLASS *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_CLASS");
  }
  std::ostringstream result;
  result << indent_ << "Class Declaration:\n";
  increaseIndent();

  // Nombre de la estructura
  result << indent_ << "Name: " << node->id() << "\n";

  // Tipo padre (si existe)
  if (node->fatherType()) {
    result << indent_ << "Father Type: " << *node->fatherType() << "\n";
  } else {
    result << indent_ << "No Father Type\n";
  }

  // Atributos
  result << indent_ << "Attributes:\n";
  increaseIndent();
  for (const auto &[type, name] : node->attributes().params()) {
    result << indent_ << type << " " << name << "\n";
  }
  decreaseIndent();

  // Métodos
  result << indent_ << "Methods:\n";
  increaseIndent();
  for (const auto &method : node->methods()) {
    const auto methodStr = method->accept(*this);
    if (!methodStr) {
      return createError(methodStr.error());
    }
    result << *methodStr;
  }
  decreaseIndent();

  // Constructor
  if (node->constructor()) {
    result << indent_ << "Constructor:\n";
    increaseIndent();
    const auto ctorStr = node->constructor()->accept(*this);
    if (!ctorStr) {
      return createError(ctorStr.error());
    }
    result << *ctorStr;
    decreaseIndent();
  }

  // Destructor
  if (node->destructor()) {
    result << indent_ << "Destructor:\n";
    increaseIndent();
    const auto dtorStr = node->destructor()->accept(*this);
    if (!dtorStr) {
      return createError(dtorStr.error());
    }
    result << *dtorStr;
    decreaseIndent();
  }

  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_ATTR_ACCESS *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_ATTR_ACCESS");
  }
  std::ostringstream result;
  result << indent_ << "attr access:\n";
  increaseIndent();
  result << indent_ << node->id() << "\n";
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_METHOD_CALL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_METHOD_CALL");
  }

  std::ostringstream result;
  result << indent_ << "Method Call:\n";
  increaseIndent();
  result << indent_ << "Name: " << node->id() << "\n";
  result << indent_ << "Arguments:\n";
  increaseIndent();
  for (const auto &arg : node->parameters()) {
    const auto argStr = arg->accept(*this);
    if (!argStr) {
      return createError(argStr.error());
    }
    result << *argStr;
  }
  decreaseIndent();
  decreaseIndent();

  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_METHOD_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_METHOD_DECL");
  }

  std::ostringstream result;
  result << indent_ << "Method Declaration:\n";
  increaseIndent();
  result << indent_ << "Name: " << node->id() << "\n";
  result << indent_ << "Generics: ";
  for (const auto& generic : node->generics()) {
    result << generic.name() << ", ";
  }
  result << "\n";
  result << indent_ << "Return Type: " << node->returnType() << "\n";
  result << indent_ << "Virtual: " << (node->isVirtual() ? "true" : "false")
         << "\n";
  result << indent_ << "Parameters:\n";
  increaseIndent();
  for (const auto &param : node->parameters()) {
    result << indent_ << "var " << param.first << " type " << param.second
           << "\n";
  }
  decreaseIndent();

  if (node->body()) {
    const auto bodyStr{node->body()->accept(*this)};
    if (!bodyStr) {
      return createError(bodyStr.error());
    }
    result << *bodyStr;
  } else {
    result << indent_ << "(empty body)\n";
  }
  decreaseIndent();

  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_CONSTRUCTOR_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_CONSTRUCTOR_DECL");
  }

  std::ostringstream result;
  result << indent_ << "Constructor Decl:\n";
  increaseIndent();
  result << indent_ << "Name: " << node->id() << "\n";
  result << indent_ << "Generics: ";
  for (const auto& generic : node->generics()) {
    result << generic.name() << ", ";
  }
  result << "\n";
  result << indent_ << "Parameters:\n";
  increaseIndent();
  for (const auto &param : node->parameters()) {
    result << indent_ << "var " << param.first << " type " << param.second
           << "\n";
  }
  decreaseIndent();

  if (node->body()) {
    const auto bodyStr{node->body()->accept(*this)};
    if (!bodyStr) {
      return createError(bodyStr.error());
    }
    result << *bodyStr;
  } else {
    result << indent_ << "(empty body)\n";
  }
  decreaseIndent();

  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_DESTRUCTOR_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_DESTRUCTOR_DECL");
  }

  std::ostringstream result;
  result << indent_ << "Destructor Decl:\n";
  increaseIndent();
  result << indent_ << "Name: " << node->id() << "\n";
  result << indent_ << "Body:\n";
  if (node->body()) {
    const auto bodyStr{node->body()->accept(*this)};
    if (!bodyStr) {
      return createError(bodyStr.error());
    }
    result << *bodyStr;
  } else {
    result << indent_ << "(empty body)\n";
  }
  decreaseIndent();

  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_THIS *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_THIS");
  }
  std::ostringstream result;
  result << indent_ << "this call\n";
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_CONSTRUCTOR_CALL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "Invalid AST_CONSTRUCTOR_CALL");
  }

  std::ostringstream result;
  result << indent_ << "Constructor Call:\n";
  increaseIndent();
  result << indent_ << "Name: " << node->id() << "\n";
  result << indent_ << "Arguments:\n";
  increaseIndent();
  for (const auto &arg : node->parameters()) {
    const auto argStr = arg->accept(*this);
    if (!argStr) {
      return createError(argStr.error());
    }
    result << *argStr;
  }
  decreaseIndent();
  decreaseIndent();

  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_AUTO_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_AUTO_DECL");
  }
  std::ostringstream result;
  result << indent_ << "auto decl\n";
  increaseIndent();
  result << indent_ << "id: " << node->id() << "\n";
  result << indent_ << "const: " << ((node->isConst()) ? "true" : "false")
         << "\n";
  result << indent_ << "value:\n";
  const auto val{node->value()->accept(*this)};
  if (!val) {
    return createError(val.error());
  }
  result << *val << "\n";
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_VAR_TYPED_DECL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_VAR_TYPED_DECL");
  }
  std::ostringstream result;
  result << indent_ << "var typed decl\n";
  increaseIndent();
  result << indent_ << "id: " << node->id() << "\n";
  result << indent_ << "type: " << node->varType() << "\n";
  result << indent_ << "const: " << ((node->isConst()) ? "true" : "false")
         << "\n";
  result << indent_ << "value:\n";
  const auto val{node->value()->accept(*this)};
  if (!val) {
    return createError(val.error());
  }
  result << *val << "\n";
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_VAR_CALL *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_VAR_CALL");
  }
  std::ostringstream result;
  result << indent_ << "Var call\n";
  increaseIndent();
  result << indent_ << "id: " << node->id() << "\n";
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const AST_CHAINED *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_CHAINED");
  }
  std::ostringstream result;
  result << indent_ << "Chained:\n";
  increaseIndent();
  if (!node->base()) {
    return createError(ERROR_TYPE::NULL_NODE, "print base");
  }
  const auto base{node->base()->accept(*this)};
  if (!base) {
    return createError(ERROR_TYPE::NULL_NODE, "print base boo");
  }
  result << indent_ << "Base:\n" << *base;
  result << indent_ << "Operations:\n";
  for (const auto &statement : node->operations()) {
    const auto val{statement->accept(*this)};
    if (!val) {
      return createError(val.error());
    }
    result << *val;
  }
  decreaseIndent();
  return result.str();
}

std::expected<std::string, Error>
PrintTree::visit(const Tree *tree) const noexcept {
  if (!tree) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid Tree");
  }
  std::ostringstream result;
  result << "Tree:\n";
  increaseIndent();
  const auto str{tree->root()->accept(*this)};
  if (!str) {
    return createError(str.error());
  }
  result << *str;
  decreaseIndent();
  return result.str();
}

} // namespace nicole
#include "../../inc/visitors/printTree.h"

#include "../../inc/lexicalAnalysis/type.h"
#include "../../inc/parsingAnalysis/ast/calls/variableCall.h"
#include "../../inc/parsingAnalysis/ast/conditionals/nodeIfStatement.h"
#include "../../inc/parsingAnalysis/ast/declaration/constDeclaration.h"
#include "../../inc/parsingAnalysis/ast/declaration/varDeclaration.h"
#include "../../inc/parsingAnalysis/ast/declaration/varReassignment.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralBool.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralChar.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralDouble.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralInt.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralString.h"
#include "../../inc/parsingAnalysis/ast/loops/nodeWhileStatement.h"
#include "../../inc/parsingAnalysis/ast/loops/nodeForStatement.h"
#include "../../inc/parsingAnalysis/ast/operations/nodeBinaryOp.h"
#include "../../inc/parsingAnalysis/ast/operations/nodeIncrement.h"
#include "../../inc/parsingAnalysis/ast/operations/nodeUnaryOp.h"
#include "../../inc/parsingAnalysis/ast/statements/statement.h"
#include "../../inc/parsingAnalysis/ast/statements/statementList.h"
#include "../../inc/parsingAnalysis/ast/utils/nodePrint.h"
#include "../../inc/parsingAnalysis/parsingAlgorithms/tree.h"

namespace nicole {

void PrintTree::decreaseIndent() const {
  if (indent_.size() >= 2)
    indent_ = indent_.substr(0, indent_.size() - 2);
}

std::string PrintTree::visit(const NodeLiteralBool *node) const {
  return indent_ + "Bool: " + (node->value() ? "true" : "false") + "\n";
}

std::string PrintTree::visit(const NodeLiteralChar *node) const {
  return indent_ + "Char: '" + std::to_string(node->value()) + "'\n";
}

std::string PrintTree::visit(const NodeLiteralDouble *node) const {
  return indent_ + "Double: " + std::to_string(node->value()) + "\n";
}

std::string PrintTree::visit(const NodeLiteralInt *node) const {
  return indent_ + "Integer: " + std::to_string(node->value()) + "\n";
}

std::string PrintTree::visit(const NodeLiteralString *node) const {
  return indent_ + "String: \"" + node->value() + "\"\n";
}

std::string PrintTree::visit(const NodeBinaryOp *node) const {
  std::ostringstream result;
  result << indent_ << "Binary Operation:\n";
  increaseIndent();
  result << indent_ << "Left:\n" << node->left()->accept(this);
  result << indent_ << "Operand: " << tokenTypeToString(node->op()) << "\n";
  result << indent_ << "Right:\n" << node->right()->accept(this);
  decreaseIndent();
  return result.str();
}

std::string PrintTree::visit(const NodeUnaryOp *node) const {
  std::ostringstream result;
  result << indent_ << "Unary Operation:\n";
  increaseIndent();
  result << indent_ << "Operand: " << tokenTypeToString(node->op()) << "\n";
  result << indent_ << "Expression:\n" << node->expression()->accept(this);
  decreaseIndent();
  return result.str();
}

std::string PrintTree::visit(const NodeIncrement *node) const {
  std::ostringstream result;
  result << indent_ << "Increment Operation:\n";
  increaseIndent();
  result << indent_ << "Operand: " << tokenTypeToString(node->op()) << "\n";
  result << indent_ << "Expression:\n" << node->expression()->accept(this);
  decreaseIndent();
  return result.str();
}

std::string PrintTree::visit(const NodeStatement *node) const {
  return node->expression()->accept(this);
}

std::string PrintTree::visit(const NodeVariableDeclaration *node) const {
  std::ostringstream result;
  result << indent_ << "Variable Declaration:\n";
  increaseIndent();
  result << indent_ << "Type: " << node->varType()->name() << "\n";
  result << indent_ << "Value:\n" << node->expression()->accept(this);
  decreaseIndent();
  return result.str();
}

std::string PrintTree::visit(const NodeConstDeclaration *node) const {
  std::ostringstream result;
  result << indent_ << "Constant Declaration:\n";
  increaseIndent();
  result << indent_ << "Type: " << node->varType()->name() << "\n";
  result << indent_ << "Value:\n" << node->expression()->accept(this);
  decreaseIndent();
  return result.str();
}

std::string PrintTree::visit(const NodeVariableCall *node) const {
  return indent_ + "Variable Call: " + node->id() + "\n";
}

std::string PrintTree::visit(const NodeVariableReassignment *node) const {
  std::ostringstream result;
  result << indent_ << "Variable Reassignment:\n";
  increaseIndent();
  result << indent_ << "Var: " << node->id() << "\n";
  result << indent_ << "Value:\n" << node->expression()->accept(this);
  decreaseIndent();
  return result.str();
}

std::string PrintTree::visit(const NodeIfStatement *node) const {
  std::ostringstream result;
  result << indent_ << "If Statement:\n";
  increaseIndent();
  result << indent_ << "Condition:\n" << node->condition()->accept(this);
  result << indent_ << "Body:\n" << node->body()->accept(this);
  if (node->hasElse()) {
    result << indent_ << "Else Body:\n" << node->elseBody()->accept(this);
  }
  decreaseIndent();
  return result.str();
}

std::string PrintTree::visit(const NodeWhileStatement *node) const {
  std::ostringstream result;
  result << indent_ << "While Statement:\n";
  increaseIndent();
  result << indent_ << "Condition:\n" << node->condition()->accept(this);
  result << indent_ << "Body:\n" << node->body()->accept(this);
  decreaseIndent();
  return result.str();
}

std::string PrintTree::visit(const NodeForStatement *node) const {
  std::ostringstream result;
  result << indent_ << "For Statement:\n";
  increaseIndent();
  result << indent_ << "Init:\n" << node->init()->accept(this);
  result << indent_ << "Condition:\n" << node->condition()->accept(this);
  result << indent_ << "Update:\n" << node->update()->accept(this);
  result << indent_ << "Body:\n" << node->body()->accept(this);
  decreaseIndent();
  return result.str();
}

std::string PrintTree::visit(const NodeStatementList *node) const {
  std::ostringstream result;
  result << indent_ << "Statement List:\n";
  increaseIndent();
  for (const auto &statement : *node) {
    result << statement->accept(this);
  }
  decreaseIndent();
  return result.str();
}

std::string PrintTree::visit(const NodePrint *node) const {
  std::ostringstream result;
  result << indent_ << "Print Statement:\n";
  increaseIndent();
  increaseIndent();
  result << indent_ << "Expression:\n" << node->expression()->accept(this);
  decreaseIndent();
  decreaseIndent();
  return result.str();
}

std::string PrintTree::visit(const Tree *node) const {
  std::ostringstream result;
  result << "Tree:\n";
  increaseIndent();
  result << node->root()->accept(this);
  decreaseIndent();
  return result.str();
}

std::string PrintTree::print(const Tree *tr) const { return visit(tr); }

} // namespace nicole
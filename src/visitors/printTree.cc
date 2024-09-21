#include "../../inc/visitors/printTree.h"

#include "../../inc/parsingAnalysis/ast/calls/variableCall.h"
#include "../../inc/parsingAnalysis/ast/conditionals/NodeIfStatetement.h"
#include "../../inc/parsingAnalysis/ast/declaration/constDeclaration.h"
#include "../../inc/parsingAnalysis/ast/declaration/varDeclaration.h"
#include "../../inc/parsingAnalysis/ast/declaration/varReassignment.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralBool.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralChar.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralDouble.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralInt.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralString.h"
#include "../../inc/parsingAnalysis/ast/operations/nodeBinaryOp.h"
#include "../../inc/parsingAnalysis/ast/statements/statement.h"
#include "../../inc/parsingAnalysis/ast/statements/statementList.h"
#include "../../inc/parsingAnalysis/parsingAlgorithms/tree.h"
#include <memory>
#include <string>

namespace nicole {

std::string PrintTree::visit(const NodeLiteralBool *node) const {
  return "Bool: " + std::to_string(node->value());
}

std::string PrintTree::visit(const NodeLiteralChar *node) const {
  return "Char: " + std::to_string(node->value());
}

std::string PrintTree::visit(const NodeLiteralDouble *node) const {
  return "Double: " + std::to_string(node->value());
}

std::string PrintTree::visit(const NodeLiteralInt *node) const {
  return "Integer: " + std::to_string(node->value());
}

std::string PrintTree::visit(const NodeLiteralString *node) const {
  return "String: " + node->value();
}

std::string PrintTree::visit(const NodeBinaryOp *node) const {
  Node *left{node->left()};
  Node *right{node->right()};
  std::string result{"BINARY OP:\n"};
  result += "Left:\n" + left->accept(this) + "\n";
  result += "Right:\n" + right->accept(this) + "\n";
  return result;
}

std::string PrintTree::visit(const NodeStatement *node) const {
  return node->expression()->accept(this);
}

std::string PrintTree::visit(const NodeVariableDeclaration *node) const {
  std::string result{"VAR DECLARATION:\n"};
  result += "Type:\n" + node->varType()->name() + "\n";
  result += "Value:\n" + node->expression()->accept(this) + "\n";

  return result;
}

std::string PrintTree::visit(const NodeConstDeclaration *node) const {
  std::string result{"CONST DECLARATION:\n"};
  result += "Type:\n" + node->varType()->name() + "\n";
  result += "Value:\n" + node->expression()->accept(this) + "\n";

  return result;
}

std::string PrintTree::visit(const NodeVariableCall *node) const {
  return "VAR CALL\nId: " + node->id();
}

std::string PrintTree::visit(const NodeVariableReassignment *node) const {
  std::string result{"VAR REASSIGNMENT:\n"};
  result += "Value:\n" + node->expression()->accept(this) + "\n";

  return result;
}

std::string PrintTree::visit(const NodeIfStatement *node) const {
  std::string result{"IF STATEMENT:\n"};
  result += "Condition:\n" + node->condition()->accept(this) + "\n";
  result += "Body:\n" + node->body()->accept(this) + "\n";
  if (node->hasElse())
    result += "Else body:\n" + node->elseBody()->accept(this) + "\n";

  return result;
}

std::string PrintTree::visit(const NodeStatementList *node) const {
  std::string result{"[\n"};
  for (const auto &statement : *node) {
    result += statement->accept(this) + "\n";
  }
  return result + "]";
}

std::string PrintTree::visit(const Tree *node) const {
  std::string result{"Tree:\n"};
  return result + node->root()->accept(this);
}

std::string PrintTree::print(const Tree *tr) const { return visit(tr); }
} // namespace nicole

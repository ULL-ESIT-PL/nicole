#include "../../inc/visitors/printTree.h"

#include "../../inc/parsingAnalysis/ast/declaration/selfAssignment.h"
#include "../../inc/lexicalAnalysis/type.h"
#include "../../inc/parsingAnalysis/ast/calls/functionCall.h"
#include "../../inc/parsingAnalysis/ast/calls/structConstructor.h"
#include "../../inc/parsingAnalysis/ast/calls/variableCall.h"
#include "../../inc/parsingAnalysis/ast/conditionals/nodeCase.h"
#include "../../inc/parsingAnalysis/ast/conditionals/nodeIfStatement.h"
#include "../../inc/parsingAnalysis/ast/conditionals/nodeSwitch.h"
#include "../../inc/parsingAnalysis/ast/declaration/constDeclaration.h"
#include "../../inc/parsingAnalysis/ast/declaration/nodeFunDeclaration.h"
#include "../../inc/parsingAnalysis/ast/declaration/nodeReturn.h"
#include "../../inc/parsingAnalysis/ast/declaration/structDeclaration.h"
#include "../../inc/parsingAnalysis/ast/declaration/varDeclaration.h"
#include "../../inc/parsingAnalysis/ast/declaration/varReassignment.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralBool.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralChar.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralDouble.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralFloat.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralInt.h"
#include "../../inc/parsingAnalysis/ast/literals/nodeLiteralString.h"
#include "../../inc/parsingAnalysis/ast/loops/nodeDoWhile.h"
#include "../../inc/parsingAnalysis/ast/loops/nodeForStatement.h"
#include "../../inc/parsingAnalysis/ast/loops/nodePass.h"
#include "../../inc/parsingAnalysis/ast/loops/nodeStop.h"
#include "../../inc/parsingAnalysis/ast/loops/nodeWhileStatement.h"
#include "../../inc/parsingAnalysis/ast/operations/nodeBinaryOp.h"
#include "../../inc/parsingAnalysis/ast/operations/nodeIncrement.h"
#include "../../inc/parsingAnalysis/ast/operations/nodeUnaryOp.h"
#include "../../inc/parsingAnalysis/ast/statements/statement.h"
#include "../../inc/parsingAnalysis/ast/statements/statementList.h"
#include "../../inc/parsingAnalysis/ast/utils/nodeImport.h"
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

std::string PrintTree::visit(const NodeLiteralFloat *node) const {
  return indent_ + "Float: " + std::to_string(node->value()) + "\n";
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

std::string PrintTree::visit(const NodeReturn *node) const {
  if (node->isEmptyExpression())
    return indent_ + "Return: void\n";
  return indent_ + "Return: \"" + node->expression()->accept(this) + "\"\n";
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

std::string PrintTree::visit(const NodeStructDeclaration *node) const {
  std::ostringstream result;
  result << indent_ << "Struct Declaration:\n";
  increaseIndent();
  result << indent_ << "Attributes:\n" << node->body()->accept(this);
  decreaseIndent();
  return result.str();
}

std::string PrintTree::visit(const NodeFunctionDeclaration *node) const {
  std::ostringstream result;
  result << indent_ << "Function Declaration:\n";
  increaseIndent();
  result << indent_ << "Id:\n" << node->id() << "\n";
  result << indent_ << "Attributes:\n";
  for (const auto &param : *node->parameters()) {
    result << indent_ << "Id: " << param.first
           << " Type: " << param.second->name();
  }
  result << indent_ << "Return:\n" << node->returnType()->name();
  result << indent_ << "Body:\n" << node->body()->accept(this);
  decreaseIndent();
  return result.str();
}

std::string PrintTree::visit(const NodeStructConstructor *node) const {
  std::ostringstream result;
  result << indent_ << "Struct Constructor:\n";
  increaseIndent();
  result << indent_ << "Params:\n" << node->parameters()->accept(this);
  decreaseIndent();
  return result.str();
}

std::string PrintTree::visit(const NodeVariableCall *node) const {
  return indent_ + "Variable Call: " + node->id() + "\n";
}

std::string PrintTree::visit(const NodeFunctionCall *node) const {
  std::ostringstream result;
  result << indent_ + "Function Call: " + node->id() + "\n";
  increaseIndent();
  result << indent_ << "Params: " << node->id() << "\n";
  for (const auto &param : *node) {
    increaseIndent();
    result << indent_ << "Value:\n" << param->accept(this);
    decreaseIndent();
  }
  decreaseIndent();
  return result.str();
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

std::string PrintTree::visit(const NodeSelfReassignment *node) const {
  std::ostringstream result;
  result << indent_ << "Self Reassignment:\n";
  increaseIndent();
  result << indent_ << "Var: " << node->id() << "\n";
  result << indent_ << "Op: " << tokenTypeToString(node->op()) << "\n";
  result << indent_ << "Value:\n" << node->expression()->accept(this);
  decreaseIndent();
  return result.str();
}

std::string PrintTree::visit(const NodeSwitchStatement *node) const {
  std::ostringstream result;
  result << indent_ << "Switch Statement:\n";
  increaseIndent();
  result << indent_ << "Match:\n" << node->match()->accept(this);
  for (const auto caseStatement : *node) {
    result << caseStatement->accept(this);
  }
  result << indent_ << "Default:\n" << node->defaultCase()->accept(this);
  decreaseIndent();
  return result.str();
}

std::string PrintTree::visit(const NodeCaseStatement *node) const {
  std::ostringstream result;
  result << indent_ << "Case Statement:\n";
  increaseIndent();
  result << indent_ << "Match:\n" << node->match()->accept(this);
  result << indent_ << "Body:\n" << node->body()->accept(this);
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

std::string PrintTree::visit(const NodeDoWhileStatement *node) const {
  std::ostringstream result;
  result << indent_ << "Do While Statement:\n";
  increaseIndent();
  result << indent_ << "Body:\n" << node->body()->accept(this);
  result << indent_ << "Condition:\n" << node->condition()->accept(this);
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

std::string PrintTree::visit(const NodeStop *node) const {
  std::ostringstream result;
  increaseIndent();
  result << indent_ << "Stop Statement:\n";
  decreaseIndent();
  return result.str();
}

std::string PrintTree::visit(const NodePass *node) const {
  std::ostringstream result;
  increaseIndent();
  result << indent_ << "Pass Statement:\n";
  decreaseIndent();
  return result.str();
}

std::string PrintTree::visit(const NodePrint *node) const {
  std::ostringstream result;
  result << indent_ << "Print Statement:\n";
  increaseIndent();
  increaseIndent();
  for (const auto expr : node->expressions()) {
    result << indent_ << "Expression:\n" << expr->accept(this);
  }
  decreaseIndent();
  decreaseIndent();
  return result.str();
}

std::string PrintTree::visit(const NodeImport *node) const {
  std::ostringstream result;
  result << indent_ << "Import Statement:\n";
  increaseIndent();
  result << indent_ << "File:\n" << node->fileName().string();
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
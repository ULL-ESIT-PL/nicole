#ifndef PRINT_TREE_H
#define PRINT_TREE_H

#include "visitor.h"
#include <iostream>
#include <sstream>

namespace nicole {

// Visitor pattern to visualize the AST
class PrintTree final : public Visitor<std::string> {
private:
  mutable std::string indent_; // Indentación actual

  // Función auxiliar para aumentar la indentación
  void increaseIndent() const { indent_ += "  "; }

  // Función auxiliar para disminuir la indentación
  void decreaseIndent() const;

public:
  std::string visit(const NodeLiteralBool *node) const override;

  std::string visit(const NodeLiteralChar *node) const override;

  std::string visit(const NodeLiteralFloat *node) const override;

  std::string visit(const NodeLiteralDouble *node) const override;

  std::string visit(const NodeLiteralInt *node) const override;

  std::string visit(const NodeLiteralString *node) const override;

  std::string visit(const NodeReturn *node) const override;

  std::string visit(const NodeBinaryOp *node) const override;

  std::string visit(const NodeUnaryOp *node) const override;

  std::string visit(const NodeIncrement *node) const override;

  std::string visit(const NodeStatement *node) const override;

  std::string visit(const NodeStatementList *node) const override;

  std::string visit(const NodeVariableDeclaration *node) const override;

  std::string visit(const NodeAutoDeclaration *node) const override;

  std::string visit(const NodeConstDeclaration *node) const override;

  std::string visit(const NodeStructDeclaration *node) const override;

  std::string visit(const NodeFunctionDeclaration *node) const override;

  std::string visit(const NodeStructConstructor *node) const override;

  std::string visit(const NodeStructAcces *node) const override;

  std::string visit(const NodeStructSetAttr *node) const override;

  std::string visit(const NodeVariableCall *node) const override;

  std::string visit(const NodeFunctionCall *node) const override;

  std::string visit(const NodeVariableReassignment *node) const override;

  std::string visit(const NodeSelfReassignment *node) const override;

  std::string visit(const NodeSwitchStatement *node) const override;

  std::string visit(const NodeCaseStatement *node) const override;

  std::string visit(const NodeTernary *node) const override;

  std::string visit(const NodeIfStatement *node) const override;

  std::string visit(const NodeWhileStatement *node) const override;

  std::string visit(const NodeDoWhileStatement *node) const override;

  std::string visit(const NodeForStatement *node) const override;

  std::string visit(const NodeStop *node) const override;

  std::string visit(const NodePass *node) const override;

  std::string visit(const NodePrint *node) const override;

  std::string visit(const NodeImport *node) const override;

  std::string visit(const Tree *tr) const override;

  std::string print(const Tree *tr) const;
};

} // namespace nicole

#endif
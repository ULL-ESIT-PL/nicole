#ifndef VISITOR_H
#define VISITOR_H

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>

namespace nicole {

enum class TokenType;
class Node; // Declaración adelantada
class NodeLiteralBool;
class NodeLiteralChar;
class NodeLiteralFloat;
class NodeLiteralDouble;
class NodeLiteralInt;
class NodeLiteralString;
class NodeReturn;
class NodeBinaryOp;
class NodeUnaryOp;
class NodeIncrement;
class NodeStatement;
class NodeStatementList;
class NodeParamsDeclaration;
class NodeVariableDeclaration;
class NodeConstDeclaration;
class NodeStructDeclaration;
class NodeFunctionDeclaration;
class NodeStructConstructor;
class NodeVariableCall;
class NodeFunctionCall;
class NodeVariableReassignment;
class NodeIfStatement;
class NodeWhileStatement;
class NodeForStatement;
class NodeStop;
class NodePass;
class NodePrint;
class Tree;

template<class T>
class Visitor {
public:
  virtual ~Visitor() = default;

  // Métodos específicos para los diferentes tipos de nodos
  virtual T visit(const NodeLiteralBool *node) const = 0;

  virtual T visit(const NodeLiteralChar *node) const = 0;

  virtual T visit(const NodeLiteralFloat *node) const = 0;

  virtual T visit(const NodeLiteralDouble *node) const = 0;

  virtual T visit(const NodeLiteralInt *node) const = 0;

  virtual T visit(const NodeLiteralString *node) const = 0;

  virtual T visit(const NodeReturn *node) const = 0;

  virtual T visit(const NodeBinaryOp *node) const = 0;

  virtual T visit(const NodeUnaryOp *node) const = 0;

  virtual T visit(const NodeIncrement *node) const = 0;

  virtual T visit(const NodeStatement *node) const = 0;

  virtual T visit(const NodeStatementList *node) const = 0;

  virtual T visit(const NodeVariableDeclaration *node) const = 0;

  virtual T visit(const NodeConstDeclaration *node) const = 0;

  virtual T visit(const NodeStructDeclaration *node) const = 0;

  virtual T visit(const NodeFunctionDeclaration *node) const = 0;

  virtual T visit(const NodeStructConstructor *node) const = 0;

  virtual T visit(const NodeVariableCall *node) const = 0;

  virtual T visit(const NodeFunctionCall *node) const = 0;

  virtual T visit(const NodeVariableReassignment *node) const = 0;

  virtual T visit(const NodeIfStatement *node) const = 0;

  virtual T visit(const NodeWhileStatement *node) const = 0;

  virtual T visit(const NodeForStatement *node) const = 0;

  virtual T visit(const NodeStop *node) const = 0;

  virtual T visit(const NodePass *node) const = 0;

  virtual T visit(const NodePrint *node) const = 0;

  virtual T visit(const Tree *tr) const = 0;
};

} // namespace nicole

#endif
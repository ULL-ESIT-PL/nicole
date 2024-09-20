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
class NodeLiteralDouble;
class NodeLiteralInt;
class NodeLiteralString;
class NodeBinaryOp;
class NodeStatement;
class NodeStatementList;
class NodeVariableDeclaration;
class NodeVariableCall;
class NodeVariableReassignment;
class NodeIfStatement;
class Tree;

class Visitor {
public:
  virtual ~Visitor() = default;

  // Métodos específicos para los diferentes tipos de nodos
  virtual llvm::Value *visit(const NodeLiteralBool *node,
                             llvm::BasicBlock *currentEntry) const = 0;

  virtual llvm::Value *visit(const NodeLiteralChar *node,
                             llvm::BasicBlock *currentEntry) const = 0;

  virtual llvm::Value *visit(const NodeLiteralDouble *node,
                             llvm::BasicBlock *currentEntry) const = 0;

  virtual llvm::Value *visit(const NodeLiteralInt *node,
                             llvm::BasicBlock *currentEntry) const = 0;

  virtual llvm::Value *visit(const NodeLiteralString *node,
                             llvm::BasicBlock *currentEntry) const = 0;

  virtual llvm::Value *visit(const NodeBinaryOp *node, llvm::BasicBlock *currentEntry) const = 0;

  virtual llvm::Value *visit(const NodeStatement *node, llvm::BasicBlock *currentEntry) const = 0;

  virtual llvm::Value *visit(const NodeStatementList *node,
                             llvm::BasicBlock *currentEntry) const = 0;

  virtual llvm::Value *visit(const NodeVariableDeclaration *node,
                             llvm::BasicBlock *currentEntry) const = 0;

  virtual llvm::Value *visit(const NodeVariableCall *node,
                             llvm::BasicBlock *currentEntry) const = 0;

  virtual llvm::Value *visit(const NodeVariableReassignment *node,
                             llvm::BasicBlock *currentEntry) const = 0;

  virtual llvm::Value *visit(const NodeIfStatement *node,
                             llvm::BasicBlock *currentEntry) const = 0;

  virtual llvm::Value *visit(const Tree *tr, llvm::BasicBlock *currentEntry) const = 0;
};

} // namespace nicole

#endif
#ifndef VISITOR_H
#define VISITOR_H

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

namespace nicole {

enum class TokenType;
class Node;  // Declaración adelantada
class NodeLiteralBool;
class NodeLiteralChar;
class NodeLiteralDouble;
class NodeLiteralInt;
class NodeLiteralString;
class NodeBinaryOp;
class NodeStatement;
class NodeStatementList;

class Visitor {
 public:
  virtual ~Visitor() = default;

  // Métodos específicos para los diferentes tipos de nodos
  virtual llvm::Value* visit(const NodeLiteralBool* node) const = 0;

  virtual llvm::Value* visit(const NodeLiteralChar* node) const = 0;

  virtual llvm::Value* visit(const NodeLiteralDouble* node) const = 0;

  virtual llvm::Value* visit(const NodeLiteralInt* node) const = 0;

  virtual llvm::Value* visit(const NodeLiteralString* node) const = 0;

  virtual llvm::Value* visit(const NodeBinaryOp* node) const = 0;

  virtual llvm::Value* visit(const NodeStatement* node) const = 0;

  virtual llvm::Value* visit(const NodeStatementList* node) const = 0;
};

}  // namespace nicole

#endif
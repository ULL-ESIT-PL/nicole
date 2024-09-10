#ifndef NODE_H
#define NODE_H

#include <llvm/IR/Value.h>

#include <memory>

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
#include "nodeType.h"

namespace nicole {
class Node {
 protected:
  NodeType type_;
  std::unique_ptr<Node> father_;
  llvm::LLVMContext* context_;

 public:
  Node(llvm::LLVMContext* context, const NodeType& type,
       std::unique_ptr<Node> father = nullptr)
      : context_{context}, type_{type}, father_{std::move(father)} {};

  virtual ~Node() = default;

  NodeType type() const { return type_; }

  Node* father() const { return father_.get(); }

  virtual llvm::Value* codeGeneration() const = 0;

  // virtual llvm::Value* accept(Visitor& visitor) const = 0;
};

}  // namespace nicole

#endif
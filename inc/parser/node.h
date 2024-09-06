#ifndef NODE_H
#define NODE_H

#include <llvm/IR/Value.h>
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

#include <memory>

#include "nodeType.h"
using namespace std;

namespace nicole {
class Node {
 protected:
  NodeType type_;
  shared_ptr<Node> father_;
  llvm::LLVMContext* context_;

 public:
  Node(llvm::LLVMContext* context, const NodeType& type, const shared_ptr<Node>& father = nullptr)
      : context_{context}, type_{type}, father_{father} {};
  virtual ~Node() = default;
  NodeType type() const { return type_; }
  shared_ptr<Node> father() const { return father_; }
  virtual llvm::Value* codeGeneration() const = 0;
};

}  // namespace nicole

#endif
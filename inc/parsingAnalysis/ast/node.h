#ifndef NODE_H
#define NODE_H

#include <llvm/IR/Value.h>

#include <memory>

#include "../../visitors/visitor.h"
#include "nodeType.h"

namespace nicole {

class Visitor;

class Node {
protected:
  NodeType type_;
  std::unique_ptr<Node> father_;

public:
  Node(const NodeType &type, std::unique_ptr<Node> father = nullptr)
      : type_{type}, father_{std::move(father)} {};

  virtual ~Node() = default;

  NodeType type() const { return type_; }

  Node *father() const { return father_.get(); }

  virtual llvm::Value *accept(const Visitor *visitor,
                              llvm::BasicBlock *currentEntry) const = 0;
};

} // namespace nicole

#endif
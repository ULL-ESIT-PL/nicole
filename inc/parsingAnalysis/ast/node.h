#ifndef NODE_H
#define NODE_H

#include <llvm/IR/Value.h>

#include <memory>
#include <string>

#include "../../visitors/codeGeneration.h"
#include "../../visitors/printTree.h"
#include "nodeType.h"

namespace nicole {

// Abstract class to represent a node of our AST
class Node : public std::enable_shared_from_this<Node> {
protected:
  NodeType type_;
  std::weak_ptr<Node> father_;

public:
  Node(const NodeType &type, std::shared_ptr<Node> father = nullptr)
      : type_{type}, father_{father} {};

  virtual ~Node() = default;

  NodeType type() const { return type_; }

  std::shared_ptr<Node> father() const { return father_.lock(); }

  virtual llvm::Value *accept(const CodeGeneration *visitor) const = 0;

  virtual std::string accept(const PrintTree *visitor) const = 0;
};

} // namespace nicole

#endif
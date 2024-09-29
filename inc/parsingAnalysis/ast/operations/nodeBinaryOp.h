#ifndef NODE_BINARY_OP_H
#define NODE_BINARY_OP_H

#include "../node.h"

namespace nicole {

class NodeBinaryOp final : public Node {
private:
  std::shared_ptr<Node> left_;
  TokenType operator_;
  std::shared_ptr<Node> right_;

public:
  NodeBinaryOp(std::shared_ptr<Node> left, const TokenType &op,
               std::shared_ptr<Node> right,
               std::shared_ptr<Node> father = nullptr)
      : Node{NodeType::BINARY, father}, left_{left}, operator_{op},
        right_{right} {
    
  };

  virtual ~NodeBinaryOp() = default;

  const Node *left() const { return left_.get(); }

  const Node *right() const { return right_.get(); }

  TokenType op() const { return operator_; }

  llvm::Value *accept(const CodeGeneration *visitor) const override {
    return visitor->visit(this);
  }

  std::string accept(const PrintTree *visitor) const override {
    return visitor->visit(this);
  }
};

} // namespace nicole

#endif
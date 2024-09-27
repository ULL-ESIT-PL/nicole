#ifndef NODE_BINARY_OP_H
#define NODE_BINARY_OP_H

#include "../node.h"

namespace nicole {

class NodeBinaryOp final : public Node {
private:
  std::unique_ptr<Node> left_;
  TokenType operator_;
  std::unique_ptr<Node> right_;

public:
  NodeBinaryOp(std::unique_ptr<Node> left, const TokenType &op,
               std::unique_ptr<Node> right,
               std::shared_ptr<Node> father = nullptr)
      : Node{NodeType::BINARY, father}, left_{std::move(left)},
        operator_{op}, right_{std::move(right)} {};

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
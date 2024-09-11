#ifndef NODE_BINARY_H
#define NODE_BINARY_H

#include "../node.h"

namespace nicole {
class NodeBinary final : public Node {
 private:
  std::unique_ptr<Node> left_;
  TokenType operator_;
  std::unique_ptr<Node> right_;

 public:
  NodeBinary(std::unique_ptr<Node> left, const TokenType& op,
             std::unique_ptr<Node> right,
             std::unique_ptr<Node> father = nullptr)
      : Node{NodeType::BINARY, std::move(father)},
        left_{std::move(left)},
        operator_{op},
        right_{std::move(right)} {};

  virtual ~NodeBinary() = default;

  Node* left() const { return left_.get(); }

  Node* right() const { return right_.get(); }

  TokenType op() const { return operator_; }

  llvm::Value* accept(const Visitor* visitor) const override {
    return visitor->visit(this);
  }
};

}  // namespace nicole

#endif
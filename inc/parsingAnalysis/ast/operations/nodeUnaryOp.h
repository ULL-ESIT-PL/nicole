#ifndef NODE_UNARY_OP_H
#define NODE_UNARY_OP_H

#include "../node.h"

namespace nicole {

class NodeUnaryOp final : public Node {
private:
  TokenType operator_;
  std::unique_ptr<Node> expression_;

public:
  NodeUnaryOp(const TokenType &op,
              std::unique_ptr<Node> expression,
              std::unique_ptr<Node> father = nullptr)
      : Node{NodeType::UNARY, std::move(father)}, operator_{op},
        expression_{std::move(expression)} {};

  virtual ~NodeUnaryOp() = default;

  const Node *expression() const { return expression_.get(); }

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
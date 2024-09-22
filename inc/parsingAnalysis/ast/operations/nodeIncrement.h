#ifndef NODE_INCREMENT_OP_H
#define NODE_INCREMENT_OP_H

#include "../declaration/varTable.h"
#include "../node.h"
#include <memory>

namespace nicole {

class NodeIncrement final : public Node {
private:
  TokenType operator_;
  std::unique_ptr<Node> expression_;
  std::shared_ptr<VariableTable> currentScope_;

public:
  NodeIncrement(const TokenType &op, std::unique_ptr<Node> expression,
                std::shared_ptr<VariableTable> currentScope,
                std::unique_ptr<Node> father = nullptr)
      : Node{NodeType::INCREMENT, std::move(father)}, operator_{op},
        expression_{std::move(expression)}, currentScope_{currentScope} {};

  virtual ~NodeIncrement() = default;

  const Node *expression() const { return expression_.get(); }

  TokenType op() const { return operator_; }

  std::shared_ptr<VariableTable> table() const { return currentScope_; }

  llvm::Value *accept(const CodeGeneration *visitor) const override {
    return visitor->visit(this);
  }

  std::string accept(const PrintTree *visitor) const override {
    return visitor->visit(this);
  }
};

} // namespace nicole

#endif
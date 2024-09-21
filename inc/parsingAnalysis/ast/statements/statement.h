#ifndef NODE_STATEMENT_H
#define NODE_STATEMENT_H

#include <memory>

#include "../node.h"

namespace nicole {

class NodeStatement final : public Node {
private:
  mutable std::unique_ptr<Node> expression_{};

public:
  NodeStatement(std::unique_ptr<Node> expression,
                std::unique_ptr<Node> father = nullptr)
      : Node{NodeType::STATEMENT, std::move(father)},
        expression_{std::move(expression)} {};

  ~NodeStatement() = default;

  Node *expression() const { return expression_.get(); }

  const Node *expressionReadOnly() const { return expression_.get(); }

  llvm::Value *accept(const CodeGeneration *visitor) const override {
    return visitor->visit(this);
  }

  std::string accept(const PrintTree *visitor) const override {
    return visitor->visit(this);
  }
};

} // namespace nicole

#endif
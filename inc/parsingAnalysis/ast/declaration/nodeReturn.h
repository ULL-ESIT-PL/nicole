#ifndef NODE_RETURN_H
#define NODE_RETURN_H

#include <memory>

#include "../node.h"

namespace nicole {

class NodeReturn final : public Node {
private:
  mutable std::shared_ptr<Node> expression_{};

public:
  NodeReturn(std::shared_ptr<Node> expression,
                std::shared_ptr<Node> father = nullptr)
      : Node{NodeType::RETURN, father}, expression_{expression} {
  };

  ~NodeReturn() = default;

  const Node *expression() const { return expression_.get(); }

  llvm::Value *accept(const CodeGeneration *visitor) const override {
    return visitor->visit(this);
  }

  std::string accept(const PrintTree *visitor) const override {
    return visitor->visit(this);
  }
};

} // namespace nicole

#endif
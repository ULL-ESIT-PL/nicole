#ifndef NODE_RETURN_H
#define NODE_RETURN_H

#include <memory>

#include "../node.h"
#include "llvm/Support/ErrorHandling.h"

namespace nicole {

class NodeReturn final : public Node {
private:
  mutable std::shared_ptr<Node> expression_{};

public:
  NodeReturn(std::shared_ptr<Node> expression,
             std::shared_ptr<Node> father = nullptr)
      : Node{NodeType::RETURN, father}, expression_{expression} {};

  ~NodeReturn() = default;

  bool isEmptyExpression() const { return expression_ == nullptr; }

  const Node *expression() const {
    if (!expression_) {
      return nullptr;
    }
    return expression_.get();
  }

  llvm::Value *accept(const CodeGeneration *visitor) const override {
    return visitor->visit(this);
  }

  std::string accept(const PrintTree *visitor) const override {
    return visitor->visit(this);
  }
};

} // namespace nicole

#endif
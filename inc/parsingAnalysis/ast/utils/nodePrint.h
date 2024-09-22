#ifndef NODE_PRINT_H
#define NODE_PRINT_H

#include <memory>

#include "../node.h"

namespace nicole {

class NodePrint final : public Node {
private:
  mutable std::unique_ptr<Node> expression_{};

public:
  NodePrint(std::unique_ptr<Node> expression,
                std::unique_ptr<Node> father = nullptr)
      : Node{NodeType::PRINT, std::move(father)},
        expression_{std::move(expression)} {};

  ~NodePrint() = default;

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
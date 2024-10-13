#ifndef NODE_PRINT_H
#define NODE_PRINT_H

#include <memory>

#include "../node.h"

namespace nicole {

// Since we dont have built-in libraries yet the printing is a node
class NodePrint final : public Node {
private:
  mutable std::vector<std::shared_ptr<Node>> expressions_{};

public:
  NodePrint(std::vector<std::shared_ptr<Node>> expressions,
            std::shared_ptr<Node> father = nullptr)
      : Node{NodeType::PRINT, father}, expressions_{expressions} {};

  ~NodePrint() = default;

  const std::vector<std::shared_ptr<Node>> expressions() const {
    return expressions_;
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
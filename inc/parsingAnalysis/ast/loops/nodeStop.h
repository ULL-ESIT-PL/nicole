#ifndef NODE_STOP_H
#define NODE_STOP_H

#include <memory>

#include "../node.h"

namespace nicole {

// Equivalent to break keyword in c++ or javaScript
class NodeStop final : public Node {
public:
  NodeStop(std::shared_ptr<Node> father = nullptr)
      : Node{NodeType::STOP, father} {};

  ~NodeStop() = default;

  llvm::Value *accept(const CodeGeneration *visitor) const override {
    return visitor->visit(this);
  }

  std::string accept(const PrintTree *visitor) const override {
    return visitor->visit(this);
  }
};

} // namespace nicole

#endif
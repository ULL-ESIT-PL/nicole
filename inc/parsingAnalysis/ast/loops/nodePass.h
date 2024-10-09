#ifndef NODE_PASS_H
#define NODE_PASS_H

#include <memory>

#include "../node.h"

namespace nicole {

// Equivalent to continue keyword in c++ or javaScript
class NodePass final : public Node {
public:
  NodePass(std::shared_ptr<Node> father = nullptr)
      : Node{NodeType::PASS, father} {};

  ~NodePass() = default;

  llvm::Value *accept(const CodeGeneration *visitor) const override {
    return visitor->visit(this);
  }

  std::string accept(const PrintTree *visitor) const override {
    return visitor->visit(this);
  }
};

} // namespace nicole

#endif
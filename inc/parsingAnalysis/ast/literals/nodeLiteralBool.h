#ifndef NODE_LITERAL_BOOL_H
#define NODE_LITERAL_BOOL_H

#include "../node.h"

namespace nicole {

class NodeLiteralBool final : public Node {
private:
  bool value_{};

public:
  NodeLiteralBool(const bool val, std::unique_ptr<Node> father = nullptr)
      : Node{NodeType::BOOL, std::move(father)}, value_{val} {};

  llvm::Value *accept(const CodeGeneration *visitor) const override {
    return visitor->visit(this);
  }

  std::string accept(const PrintTree *visitor) const override {
    return visitor->visit(this);
  }

  bool value() const { return value_; }
};

} // namespace nicole

#endif
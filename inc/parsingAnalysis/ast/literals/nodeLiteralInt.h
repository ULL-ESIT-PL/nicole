#ifndef NODE_LITERAL_INT_H
#define NODE_LITERAL_INT_H

#include "../node.h"

namespace nicole {

class NodeLiteralInt final : public Node {
private:
  int value_{};

public:
  NodeLiteralInt(const int val, std::shared_ptr<Node> father = nullptr)
      : Node{NodeType::INT, father}, value_{val} {};
      
  llvm::Value *accept(const CodeGeneration *visitor) const override {
    return visitor->visit(this);
  }

  std::string accept(const PrintTree *visitor) const override {
    return visitor->visit(this);
  }

  int value() const { return value_; }
};

} // namespace nicole

#endif
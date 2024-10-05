#ifndef NODE_LITERAL_DOUBLE_H
#define NODE_LITERAL_DOUBLE_H

#include "../node.h"

namespace nicole {

class NodeLiteralDouble final : public Node {
private:
  double value_{};

public:
  NodeLiteralDouble(const double val, std::shared_ptr<Node> father = nullptr)
      : Node{NodeType::DOUBLE, father}, value_{val} {};

  llvm::Value *accept(const CodeGeneration *visitor) const override {
    return visitor->visit(this);
  }

  std::string accept(const PrintTree *visitor) const override {
    return visitor->visit(this);
  }

  double value() const { return value_; }
};

} // namespace nicole

#endif
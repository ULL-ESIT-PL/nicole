#ifndef NODE_LITERAL_FLOAT_H
#define NODE_LITERAL_FLOAT_H

#include "../node.h"

namespace nicole {

class NodeLiteralFloat final : public Node {
private:
  double value_{};

public:
  NodeLiteralFloat(const double val, std::shared_ptr<Node> father = nullptr)
      : Node{NodeType::FLOAT, father}, value_{val} {};

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
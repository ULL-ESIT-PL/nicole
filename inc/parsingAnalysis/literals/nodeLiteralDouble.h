#ifndef NODE_LITERAL_DOUBLE_H
#define NODE_LITERAL_DOUBLE_H

#include "../node.h"

namespace nicole {
class NodeLiteralDouble final : public Node {
 private:
  double value_{};

 public:
  NodeLiteralDouble(const double val, std::unique_ptr<Node> father = nullptr)
      : Node{NodeType::DOUBLE, std::move(father)}, value_{val} {};

  llvm::Value* accept(const Visitor* visitor) const override {
    return visitor->visit(this);
  }

  double value() const { return value_; }
};

}  // namespace nicole

#endif
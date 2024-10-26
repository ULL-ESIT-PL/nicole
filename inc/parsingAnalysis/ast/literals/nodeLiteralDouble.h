#ifndef NODE_LITERAL_DOUBLE_H
#define NODE_LITERAL_DOUBLE_H

#include "../node.h"
#include "../../types/doubleType.h"
#include "typedExpression.h"

namespace nicole {

class NodeLiteralDouble final : public Node, TypedExpression {
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

  std::shared_ptr<GenericType> type() const override { return std::make_shared<DoubleType>(); }

  double value() const { return value_; }
};

} // namespace nicole

#endif
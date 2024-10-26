#ifndef NODE_LITERAL_FLOAT_H
#define NODE_LITERAL_FLOAT_H

#include "../node.h"
#include "../../types/floatType.h"
#include "typedExpression.h"

namespace nicole {

class NodeLiteralFloat final : public Node, TypedExpression {
private:
  float value_{};

public:
  NodeLiteralFloat(const float val, std::shared_ptr<Node> father = nullptr)
      : Node{NodeType::FLOAT, father}, value_{val} {};

  llvm::Value *accept(const CodeGeneration *visitor) const override {
    return visitor->visit(this);
  }

  std::string accept(const PrintTree *visitor) const override {
    return visitor->visit(this);
  }

  std::shared_ptr<GenericType> type() const override { return std::make_shared<FloatType>(); }

  float value() const { return value_; }
};

} // namespace nicole

#endif
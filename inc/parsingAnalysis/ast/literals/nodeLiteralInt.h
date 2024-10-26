#ifndef NODE_LITERAL_INT_H
#define NODE_LITERAL_INT_H

#include "../node.h"
#include "../../types/intType.h"
#include "typedExpression.h"

namespace nicole {

class NodeLiteralInt final : public Node, TypedExpression {
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

  std::shared_ptr<GenericType> type() const override { return std::make_shared<IntType>(); }

  int value() const { return value_; }
};

} // namespace nicole

#endif
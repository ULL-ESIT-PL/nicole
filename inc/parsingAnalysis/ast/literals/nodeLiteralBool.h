#ifndef NODE_LITERAL_BOOL_H
#define NODE_LITERAL_BOOL_H

#include "../node.h"
#include "typedExpression.h"
#include "../../types/boolType.h"
#include <memory>

namespace nicole {

class NodeLiteralBool final : public Node, TypedExpression {
private:
  bool value_{};

public:
  NodeLiteralBool(const bool val, std::shared_ptr<Node> father = nullptr)
      : Node{NodeType::BOOL, father}, value_{val} {};

  llvm::Value *accept(const CodeGeneration *visitor) const override {
    return visitor->visit(this);
  }

  std::string accept(const PrintTree *visitor) const override {
    return visitor->visit(this);
  }

  std::shared_ptr<GenericType> type() const override { return std::make_shared<BoolType>(); }

  bool value() const { return value_; }
};

} // namespace nicole

#endif
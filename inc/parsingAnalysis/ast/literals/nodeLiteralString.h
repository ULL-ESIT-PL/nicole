#ifndef NODE_LITERAL_STRING_H
#define NODE_LITERAL_STRING_H

#include "../node.h"
#include "../../types/stringType.h"
#include "typedExpression.h"

namespace nicole {

class NodeLiteralString final : public Node, TypedExpression {
private:
  std::string value_{};

  std::string unEscape(const std::string &val) const;

public:
  NodeLiteralString(const std::string &val,
                    std::shared_ptr<Node> father = nullptr)
      : Node{NodeType::STRING, father} {
    value_ = unEscape(val);
  };

  llvm::Value *accept(const CodeGeneration *visitor) const override {
    return visitor->visit(this);
  }

  std::string accept(const PrintTree *visitor) const override {
    return visitor->visit(this);
  }

  std::shared_ptr<GenericType> type() const override { return std::make_shared<StringType>(); }

  std::string value() const { return value_; }
};

} // namespace nicole

#endif
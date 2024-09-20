#ifndef NODE_LITERAL_INT_H
#define NODE_LITERAL_INT_H

#include "../node.h"

namespace nicole {

class NodeLiteralInt final : public Node {
private:
  int value_{};

public:
  NodeLiteralInt(const int val, std::unique_ptr<Node> father = nullptr)
      : Node{NodeType::INT, std::move(father)}, value_{val} {};

  llvm::Value *accept(const Visitor *visitor, llvm::BasicBlock *currentEntry) const override {
    return visitor->visit(this, currentEntry);
  }

  int value() const { return value_; }
};

} // namespace nicole

#endif
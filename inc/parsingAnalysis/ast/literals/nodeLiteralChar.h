#ifndef NODE_LITERAL_CHAR_H
#define NODE_LITERAL_CHAR_H

#include "../node.h"

namespace nicole {

class NodeLiteralChar final : public Node {
private:
  char value_{};

public:
  NodeLiteralChar(const char val, std::unique_ptr<Node> father = nullptr)
      : Node{NodeType::CHAR, std::move(father)}, value_{val} {};

  llvm::Value *accept(const CodeGeneration *visitor) const override {
    return visitor->visit(this);
  }

  std::string accept(const PrintTree *visitor) const override {
    return visitor->visit(this);
  }

  char value() const { return value_; }
};

} // namespace nicole

#endif
#ifndef NODE_LITERAL_STRING_H
#define NODE_LITERAL_STRING_H

#include "../node.h"

namespace nicole {

class NodeLiteralString final : public Node {
private:
  std::string value_{};

public:
  NodeLiteralString(const std::string &val,
                    std::unique_ptr<Node> father = nullptr)
      : Node{NodeType::STRING, std::move(father)}, value_{val} {};

  llvm::Value *accept(const Visitor *visitor, llvm::BasicBlock *currentEntry,
                      llvm::Module *currentModule) const override {
    return visitor->visit(this, currentEntry, currentModule);
  }

  std::string value() const { return value_; }
};

} // namespace nicole

#endif
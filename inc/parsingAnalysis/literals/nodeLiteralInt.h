#ifndef NODE_LITERAL_INT_H
#define NODE_LITERAL_INT_H

#include "../node.h"

namespace nicole {

class NodeLiteralInt final : public Node {
 private:
  int value_{};

 public:
  NodeLiteralInt(llvm::LLVMContext* context, const int val,
                 std::unique_ptr<Node> father = nullptr)
      : Node{context, NodeType::INT, std::move(father)}, value_{val} {};

  llvm::Value* accept(const std::unique_ptr<Visitor>& visitor) const override {
    return visitor->visit(std::make_unique<NodeLiteralInt>(*this));
  }

  llvm::Value* codeGeneration() const override;

  int value() const { return value_; }
};

}  // namespace nicole

#endif
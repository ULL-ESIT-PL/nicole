#ifndef NODE_BINARY_H
#define NODE_BINARY_H

#include "../node.h"

namespace nicole {
class NodeBinary final : public Node {
 private:
  std::unique_ptr<Node> left_;
  Operator operator_;
  std::unique_ptr<Node> right_;

 public:
  NodeBinary(llvm::LLVMContext* context, std::unique_ptr<Node> left,
             const Operator& op, std::unique_ptr<Node> right,
             std::unique_ptr<Node> father = nullptr)
      : Node{context, NodeType::BINARY, std::move(father)},
        left_{std::move(left)},
        operator_{op},
        right_{std::move(right)} {};

  virtual ~NodeBinary() = default;

  Node* left() const { return left_.get(); }

  Node* right() const { return right_.get(); }

  Operator op() const { return operator_; }

  llvm::Value* codeGeneration() const override;

  llvm::Value* accept(const std::unique_ptr<Visitor>& visitor) const override {
    return visitor->visit(std::make_unique<NodeBinary>(*this));
  }
};

}  // namespace nicole

#endif
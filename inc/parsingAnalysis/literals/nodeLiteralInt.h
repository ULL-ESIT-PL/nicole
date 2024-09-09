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

  ~NodeLiteralInt() = default;

  llvm::Value* codeGeneration() const override;
};

}  // namespace nicole

#endif
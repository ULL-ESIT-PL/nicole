#ifndef NODE_LITERAL_BOOL_H
#define NODE_LITERAL_BOOL_H

#include "../node.h"

namespace nicole {
class NodeLiteralBool final : public Node {
 private:
  bool value_{};

 public:
  NodeLiteralBool(llvm::LLVMContext* context, const bool val,
                  std::unique_ptr<Node> father = nullptr)
      : Node{context, NodeType::BOOL, std::move(father)}, value_{val} {};

  llvm::Value* codeGeneration() const override;
};

}  // namespace nicole

#endif
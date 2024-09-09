#ifndef NODE_LITERAL_CHAR_H
#define NODE_LITERAL_CHAR_H

#include "../node.h"

namespace nicole {
class NodeLiteralChar final : public Node {
 private:
  char value_{};

 public:
  NodeLiteralChar(llvm::LLVMContext* context, const char val,
                  std::unique_ptr<Node> father = nullptr)
      : Node{context, NodeType::CHAR, std::move(father)}, value_{val} {};

  llvm::Value* codeGeneration() const override;
};

}  // namespace nicole

#endif
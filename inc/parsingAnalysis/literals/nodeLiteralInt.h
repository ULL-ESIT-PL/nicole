#ifndef NODE_LITERAL_INT_H
#define NODE_LITERAL_INT_H

#include <memory>

#include "../node.h"

namespace nicole {
class NodeLiteralInt: public Node {
 private:
    int value_{};
 public:
  NodeLiteralInt(llvm::LLVMContext* context, const int val, const std::shared_ptr<Node>& father = nullptr)
      : Node{context, NodeType::INT, father}, value_{val} {};
  ~NodeLiteralInt() = default;
  llvm::Value* codeGeneration() const override;
};

}  // namespace nicole

#endif
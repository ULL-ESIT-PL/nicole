#ifndef NODE_LITERAL_BOOL_H
#define NODE_LITERAL_BOOL_H

#include <memory>

#include "../node.h"

namespace nicole {
class NodeLiteralBool : public Node {
 private:
  bool value_{};

 public:
  NodeLiteralBool(llvm::LLVMContext* context, const bool val,
                  const std::shared_ptr<Node>& father = nullptr)
      : Node{context, NodeType::BOOL, father}, value_{val} {};

  ~NodeLiteralBool() = default;
  
  llvm::Value* codeGeneration() const override;
};

}  // namespace nicole

#endif
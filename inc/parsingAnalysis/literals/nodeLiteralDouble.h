#ifndef NODE_LITERAL_DOUBLE_H
#define NODE_LITERAL_DOUBLE_H

#include "../node.h"

namespace nicole {
class NodeLiteralDouble final : public Node {
 private:
  double value_{};

 public:
  NodeLiteralDouble(llvm::LLVMContext* context, const double val,
                    std::unique_ptr<Node> father = nullptr)
      : Node{context, NodeType::DOUBLE, std::move(father)}, value_{val} {};

  llvm::Value* codeGeneration() const override;
};

}  // namespace nicole

#endif
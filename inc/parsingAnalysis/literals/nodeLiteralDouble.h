#ifndef NODE_LITERAL_DOUBLE_H
#define NODE_LITERAL_DOUBLE_H

#include <memory>

#include "../node.h"

namespace nicole {
class NodeLiteralDouble: public Node {
 private:
    double value_{};
 public:
  NodeLiteralDouble(llvm::LLVMContext* context, const double val, const std::shared_ptr<Node>& father = nullptr)
      : Node{context, NodeType::DOUBLE, father}, value_{val} {};

  ~NodeLiteralDouble() = default;
  
  llvm::Value* codeGeneration() const override;
};

}  // namespace nicole

#endif
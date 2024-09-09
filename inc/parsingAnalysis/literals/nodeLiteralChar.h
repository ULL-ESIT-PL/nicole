#ifndef NODE_LITERAL_CHAR_H
#define NODE_LITERAL_CHAR_H

#include <memory>

#include "../node.h"

namespace nicole {
class NodeLiteralChar: public Node {
 private:
    char value_{};
 public:
  NodeLiteralChar(llvm::LLVMContext* context, const char val, const std::shared_ptr<Node>& father = nullptr)
      : Node{context, NodeType::CHAR, father}, value_{val} {};

  ~NodeLiteralChar() = default;
  
  llvm::Value* codeGeneration() const override;
};

}  // namespace nicole

#endif
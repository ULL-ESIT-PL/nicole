#ifndef NODE_LITERAL_STRING_H
#define NODE_LITERAL_STRING_H

#include "../node.h"

namespace nicole {
class NodeLiteralString final : public Node {
 private:
  std::string value_{};
  llvm::Module* module_;

 public:
  NodeLiteralString(llvm::LLVMContext* context, llvm::Module* module,
                    const std::string& val,
                    std::unique_ptr<Node> father = nullptr)
      : Node{context, NodeType::STRING, std::move(father)},
        module_{module},
        value_{val} {};

  llvm::Value* codeGeneration() const override;
};

}  // namespace nicole

#endif
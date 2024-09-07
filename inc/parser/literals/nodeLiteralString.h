#ifndef NODE_LITERAL_STRING_H
#define NODE_LITERAL_STRING_H

#include <memory>

#include "../node.h"
using namespace std;

namespace nicole {
class NodeLiteralString: public Node {
 private:
    std::string value_{};
    llvm::Module* module_;
 public:
  NodeLiteralString(llvm::LLVMContext* context, llvm::Module* module, const std::string& val, const shared_ptr<Node>& father = nullptr)
      : Node{context, NodeType::STRING, father}, module_{module}, value_{val} {};
  ~NodeLiteralString() = default;
  llvm::Value* codeGeneration() const override;
};

}  // namespace nicole

#endif
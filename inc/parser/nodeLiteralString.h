#ifndef NODE_LITERAL_STRING_H
#define NODE_LITERAL_STRING_H

#include <memory>

#include "node.h"
using namespace std;

namespace nicole {
class NodeLiteralString: public Node {
 private:
    std::string value_{};
 public:
  NodeLiteralString(llvm::LLVMContext* context, const std::string& val, const shared_ptr<Node>& father = nullptr)
      : Node{context, NodeType::STRING, father}, value_{val} {};
  ~NodeLiteralString() = default;
  llvm::Value* codeGeneration() const override;
};

}  // namespace nicole

#endif
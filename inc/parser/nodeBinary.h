#ifndef NODE_BINARY_H
#define NODE_BINARY_H

#include <memory>
#include <string>

#include "node.h"
using namespace std;

namespace nicole {
class NodeBinary : public Node {
 private:
  Node* left_;
  Operator operator_;
  Node* right_;

 public:
  NodeBinary(llvm::LLVMContext* context, Node* left,
             const Operator& op, Node* right,
             const shared_ptr<Node>& father = nullptr)
      : Node{context, NodeType::BINARY, father},
        left_{left},
        operator_{op},
        right_{right} {};
  virtual ~NodeBinary() = default;
  llvm::Value* codeGeneration() const override;
};

}  // namespace nicole

#endif
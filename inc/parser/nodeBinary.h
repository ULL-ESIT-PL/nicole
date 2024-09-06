#ifndef NODE_BINARY_H
#define NODE_BINARY_H

#include <memory>
#include <string>

#include "nodeLiteral.h"
using namespace std;

namespace nicole {
class NodeBinary : public Node {
 private:
  NodeLiteral* left_;
  Operator operator_;
  NodeLiteral* right_;

 public:
  NodeBinary(llvm::LLVMContext* context, NodeLiteral* left,
             const Operator& op, NodeLiteral* right,
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
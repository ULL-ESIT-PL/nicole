#ifndef NODE_LITERAL_H
#define NODE_LITERAL_H

#include <memory>
#include <llvm/IR/Value.h>

#include "node.h"
using namespace std;

namespace nicole {
class NodeLiteral: public Node {
 private:
    double value_{};
 public:
  NodeLiteral(llvm::LLVMContext* context, const double val, const shared_ptr<Node>& father = nullptr)
      : Node{context, NodeType::LITERAL, father}, value_{val} {};
  virtual ~NodeLiteral() = default;
  llvm::Value* codeGeneration() const override;
};

}  // namespace nicole

#endif
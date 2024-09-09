#ifndef NODE_STATEMENT_H
#define NODE_STATEMENT_H

#include <memory>
#include <vector>

#include "../node.h"
using namespace std;

namespace nicole {
class NodeStatement final : public Node {
 private:
  std::unique_ptr<Node> expression_{};

 public:
  NodeStatement(llvm::LLVMContext* context,
                std::unique_ptr<Node> expression,
                std::unique_ptr<Node> father = nullptr)
      : Node{context, NodeType::STATEMENT, std::move(father)},
        expression_{std::move(expression)} {};
  ~NodeStatement() = default;
  
  llvm::Value* codeGeneration() const override;
};

}  // namespace nicole

#endif
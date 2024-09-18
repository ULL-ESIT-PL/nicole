#ifndef NODE_STATEMENT_H
#define NODE_STATEMENT_H

#include <memory>

#include "../node.h"

namespace nicole {

class NodeStatement final : public Node {
private:
  std::unique_ptr<Node> expression_{};

public:
  NodeStatement(std::unique_ptr<Node> expression,
                std::unique_ptr<Node> father = nullptr)
      : Node{NodeType::STATEMENT, std::move(father)},
        expression_{std::move(expression)} {};

  ~NodeStatement() = default;

  Node *expression() const { return expression_.get(); }

  llvm::Value *accept(const Visitor *visitor, llvm::BasicBlock *currentEntry,
                      llvm::Module *currentModule) const override {
    return visitor->visit(this, currentEntry, currentModule);
  }
};

} // namespace nicole

#endif
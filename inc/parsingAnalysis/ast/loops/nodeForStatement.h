#ifndef NODE_FOR_STATEMENT_H
#define NODE_FOR_STATEMENT_H

#include "../statements/statementList.h"
#include "llvm/Support/ErrorHandling.h"

namespace nicole {

class NodeForStatement final : public Node {
private:
  mutable std::unique_ptr<NodeStatementList> init_;
  mutable std::unique_ptr<Node> condition_;
  mutable std::unique_ptr<NodeStatementList> update_;
  mutable std::unique_ptr<NodeStatementList> body_;

public:
  NodeForStatement(std::unique_ptr<NodeStatementList> init,
                   std::unique_ptr<Node> condition,
                   std::unique_ptr<NodeStatementList> update,
                   std::unique_ptr<NodeStatementList> body,
                   std::unique_ptr<Node> father = nullptr)
      : Node{NodeType::FOR, std::move(father)}, init_{std::move(init)},
        condition_{std::move(condition)}, update_{std::move(update)},
        body_{std::move(body)} {};

  const NodeStatementList *init() const { return init_.get(); }

  const Node *condition() const { return condition_.get(); }

  const NodeStatementList *update() const { return update_.get(); }

  const NodeStatementList *body() const { return body_.get(); }

  llvm::Value *accept(const CodeGeneration *visitor) const override {
    return visitor->visit(this);
  }

  std::string accept(const PrintTree *visitor) const override {
    return visitor->visit(this);
  }
};

} // namespace nicole

#endif
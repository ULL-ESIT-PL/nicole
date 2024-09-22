#ifndef NODE_WHILE_STATEMENT_H
#define NODE_WHILE_STATEMENT_H

#include "../statements/statementList.h"

namespace nicole {

class NodeWhileStatement final : public Node {
private:
  mutable std::unique_ptr<Node> condition_;
  mutable std::unique_ptr<NodeStatementList> body_;

public:
  NodeWhileStatement(std::unique_ptr<Node> condition,
                     std::unique_ptr<NodeStatementList> body,
                     std::unique_ptr<Node> father = nullptr)
      : Node{NodeType::WHILE, std::move(father)},
        condition_{std::move(condition)}, body_{std::move(body)} {};

  const Node *condition() const { return condition_.get(); }

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
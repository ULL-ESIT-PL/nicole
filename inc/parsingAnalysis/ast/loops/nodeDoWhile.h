#ifndef NODE_DO_WHILE_STATEMENT_H
#define NODE_DO_WHILE_STATEMENT_H

#include "../statements/statementList.h"

namespace nicole {

class NodeDoWhileStatement final : public Node {
private:
  mutable std::shared_ptr<Node> condition_;
  mutable std::shared_ptr<NodeStatementList> body_;

public:
  NodeDoWhileStatement(std::shared_ptr<Node> condition,
                     std::shared_ptr<NodeStatementList> body,
                     std::shared_ptr<Node> father = nullptr)
      : Node{NodeType::DO_WHILE, father}, condition_{condition}, body_{body} {};

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
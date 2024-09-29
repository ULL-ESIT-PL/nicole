#ifndef NODE_IF_STATEMENT_H
#define NODE_IF_STATEMENT_H

#include "../node.h"
#include "../statements/statementList.h"
#include <memory>

namespace nicole {

class NodeIfStatement final : public Node {
private:
  mutable std::shared_ptr<Node> condition_;
  mutable std::shared_ptr<NodeStatementList> body_;
  mutable std::shared_ptr<NodeStatementList> elseBody_;

public:
  NodeIfStatement(std::shared_ptr<Node> condition,
                  std::shared_ptr<NodeStatementList> body,
                  std::shared_ptr<NodeStatementList> elseBody = nullptr,
                  std::shared_ptr<Node> father = nullptr)
      : Node{NodeType::IF, father}, condition_{condition}, body_{body},
        elseBody_{elseBody} {};

  const Node *condition() const { return condition_.get(); }

  const NodeStatementList *body() const { return body_.get(); }

  bool hasElse() const { return elseBody_ != nullptr; }

  const NodeStatementList *elseBody() const { return elseBody_.get(); }

  llvm::Value *accept(const CodeGeneration *visitor) const override {
    return visitor->visit(this);
  }

  std::string accept(const PrintTree *visitor) const override {
    return visitor->visit(this);
  }
};

} // namespace nicole

#endif
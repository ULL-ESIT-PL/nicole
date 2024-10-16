#ifndef NODE_CASE_STATEMENT_H
#define NODE_CASE_STATEMENT_H

#include "../node.h"
#include "../statements/statementList.h"
#include <memory>
#include <vector>

namespace nicole {

class NodeCaseStatement final : public Node {
private:
  mutable std::shared_ptr<Node> match_;
  mutable std::shared_ptr<NodeStatementList> body_;

public:
  NodeCaseStatement(std::shared_ptr<Node> match,
                    std::shared_ptr<NodeStatementList> body,
                    std::shared_ptr<Node> father = nullptr)
      : Node{NodeType::CASE, father}, match_{match}, body_{body} {};

  const Node *match() const { return match_.get(); }

  const NodeStatementList* body() const { return body_.get(); }

  llvm::Value *accept(const CodeGeneration *visitor) const override {
    return visitor->visit(this);
  }

  std::string accept(const PrintTree *visitor) const override {
    return visitor->visit(this);
  }
};

} // namespace nicole

#endif
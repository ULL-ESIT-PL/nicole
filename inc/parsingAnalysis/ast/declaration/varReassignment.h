#ifndef VARIABLE_REASSIGNMENT_H
#define VARIABLE_REASSIGNMENT_H

#include "../../types/genericType.h"
#include "../node.h"
#include "varTable.h"

namespace nicole {

class NodeVariableReassignment : public Node {
private:
  /* data */
  std::string id_{""};
  std::unique_ptr<Node> expression_;
  std::shared_ptr<VariableTable> currentScope_;

public:
  NodeVariableReassignment(const std::string &id,
                           std::unique_ptr<Node> expression,
                           std::shared_ptr<VariableTable> currentScope,
                           std::unique_ptr<Node> father = nullptr)
      : Node{NodeType::VAR_REG, std::move(father)}, id_{id},
        currentScope_{currentScope}, expression_{std::move(expression)} {};

  std::string id() const { return id_; }

  const Node *expression() const { return expression_.get(); }

  std::shared_ptr<VariableTable> table() const { return currentScope_; }

  llvm::Value *accept(const CodeGeneration *visitor) const override {
    return visitor->visit(this);
  }

  std::string accept(const PrintTree *visitor) const override {
    return visitor->visit(this);
  }
};

} // namespace nicole

#endif
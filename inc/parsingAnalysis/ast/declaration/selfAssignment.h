#ifndef SELF_REASSIGNMENT_H
#define SELF_REASSIGNMENT_H

#include "../../types/typeTable.h"
#include "../node.h"
#include "varTable.h"

namespace nicole {

class NodeSelfReassignment : public Node {
private:
  /* data */
  std::string id_{""};
  TokenType op_{};
  std::shared_ptr<Node> expression_;
  std::shared_ptr<VariableTable> currentScope_;
  std::shared_ptr<TypeTable> typeTable_;

public:
  NodeSelfReassignment(const std::string &id,
                           const TokenType op,
                           std::shared_ptr<Node> expression,
                           std::shared_ptr<VariableTable> currentScope,
                           std::shared_ptr<TypeTable> typeTable,
                           std::shared_ptr<Node> father = nullptr)
      : Node{NodeType::SELF_REG, father}, id_{id}, op_{op}, expression_{expression},
        currentScope_{currentScope}, typeTable_{typeTable} {};

  std::string id() const { return id_; }

  TokenType op() const { return op_; }

  const Node *expression() const { return expression_.get(); }

  std::shared_ptr<VariableTable> table() const { return currentScope_; }

  std::shared_ptr<TypeTable> typeTable() const { return typeTable_; }

  llvm::Value *accept(const CodeGeneration *visitor) const override {
    return visitor->visit(this);
  }

  std::string accept(const PrintTree *visitor) const override {
    return visitor->visit(this);
  }
};

} // namespace nicole

#endif
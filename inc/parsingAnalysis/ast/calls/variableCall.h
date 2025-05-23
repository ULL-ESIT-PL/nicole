#ifndef VARIABLE_CALL_H
#define VARIABLE_CALL_H

#include "../declaration/varTable.h"
#include "../node.h"

namespace nicole {

class NodeVariableCall : public Node {
private:
  /* data */
  std::string id_{""};
  std::shared_ptr<VariableTable> currentScope_;

public:
  NodeVariableCall(const std::string &id,
                   std::shared_ptr<VariableTable> currentScope,
                   std::shared_ptr<Node> father = nullptr)
      : Node{NodeType::CALL_VAR, father}, id_{id},
        currentScope_{currentScope} {};

  std::string id() const { return id_; }

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
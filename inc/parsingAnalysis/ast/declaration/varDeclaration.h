#ifndef VARIABLE_DECLARATION_H
#define VARIABLE_DECLARATION_H

#include "../../types/genericType.h"
#include "../node.h"
#include "varTable.h"

namespace nicole {

class NodeVariableDeclaration : public Node {
private:
  /* data */
  std::string id_{""};
  mutable std::shared_ptr<GenericType> varType_;
  std::shared_ptr<Node> expression_;
  std::shared_ptr<VariableTable> currentScope_;

public:
  NodeVariableDeclaration(const std::string &id,
                          std::shared_ptr<GenericType> varType,
                          std::shared_ptr<Node> expression,
                          std::shared_ptr<VariableTable> currentScope,
                          std::shared_ptr<Node> father = nullptr)
      : Node{NodeType::VAR_DECL, father}, id_{id}, varType_{varType},
        expression_{expression}, currentScope_{currentScope} {};

  std::string id() const { return id_; }

  const GenericType *varType() const { return varType_.get(); }

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
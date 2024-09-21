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
  mutable std::unique_ptr<GenericType> varType_;
  std::unique_ptr<Node> expression_;
  std::shared_ptr<VariableTable> currentScope_;

public:
  NodeVariableDeclaration(const std::string &id,
                          std::unique_ptr<GenericType> varType,
                          std::unique_ptr<Node> expression,
                          std::shared_ptr<VariableTable> currentScope,
                          std::unique_ptr<Node> father = nullptr)
      : Node{NodeType::VAR_DECL, std::move(father)}, id_{id},
        varType_{std::move(varType)}, expression_{std::move(expression)},
        currentScope_{currentScope} {};

  std::string id() const { return id_; }

  std::unique_ptr<GenericType> varType() const { return std::move(varType_); }

  Node *expression() const { return expression_.get(); }

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
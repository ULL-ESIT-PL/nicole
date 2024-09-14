#ifndef VARIABLE_DECLARATION
#define VARIABLE_DECLARATION

#include "../node.h"
#include "../types/genericType.h"
#include "varTable.h"

namespace nicole {

class NodeVariableDeclaration : public Node {
 private:
  /* data */
  std::string id_{""};
  std::unique_ptr<GenericType> varType_;
  std::unique_ptr<Node> expression_;
  std::shared_ptr<VariableTable> currentScope_;

 public:
  NodeVariableDeclaration(const std::string& id,
                          std::unique_ptr<GenericType> varType,
                          std::unique_ptr<Node> expression,
                          std::shared_ptr<VariableTable> currentScope,
                          std::unique_ptr<Node> father = nullptr)
      : Node{NodeType::VAR_DECL, std::move(father)},
        id_{id},
        varType_{std::move(varType)},
        currentScope_{currentScope},
        expression_{std::move(expression)} {
    // ONLY currentScope_ is owned by this node, the rest are owned by the scope
    currentScope_->addVariable(id, std::move(varType_), std::move(expression_));
  };

  std::string id() const { return id_; }

  GenericType* type() const { return varType_.get(); }

  Node* expression() const { return expression_.get(); }

  std::shared_ptr<VariableTable> table() const { return currentScope_; }

  llvm::Value* accept(const Visitor* visitor) const override {
    return visitor->visit(this);
  }
};

}  // namespace nicole

#endif
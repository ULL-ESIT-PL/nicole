#ifndef FUNCTION_DECLARATION_H
#define FUNCTION_DECLARATION_H

#include "../../types/typeTable.h"
#include "../node.h"
#include "paramsDeclaration.h"
#include "varTable.h"
#include <memory>

namespace nicole {

class NodeFunctionDeclaration : public Node {
private:
  /* data */
  std::string id_{""};
  std::shared_ptr<ParamsDeclaration> params_;
  mutable std::shared_ptr<GenericType> returnType_;
  std::shared_ptr<NodeStatementList> body_;
  std::shared_ptr<VariableTable> currentScope_;
  std::shared_ptr<TypeTable> typeTable_;

public:
  NodeFunctionDeclaration(const std::string &id,
                          std::shared_ptr<ParamsDeclaration> params,
                          std::shared_ptr<GenericType> returnType,
                          std::shared_ptr<NodeStatementList> body,
                          std::shared_ptr<VariableTable> currentScope,
                          std::shared_ptr<TypeTable> typeTable,
                          std::shared_ptr<Node> father = nullptr)
      : Node{NodeType::FUN_DECL, father}, id_{id}, params_{params},
        returnType_{returnType}, body_{body}, currentScope_{currentScope},
        typeTable_{typeTable} {};

  std::string id() const { return id_; }

  const ParamsDeclaration *parameters() const { return params_.get(); }

  const GenericType *returnType() const { return returnType_.get(); }

  const NodeStatementList *body() const { return body_.get(); }

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
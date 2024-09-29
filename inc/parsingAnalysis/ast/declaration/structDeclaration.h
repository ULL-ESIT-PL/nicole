#ifndef NODE_STRUCT_DECLARATION_H
#define NODE_STRUCT_DECLARATION_H

#include "../../types/genericType.h"
#include "../node.h"
#include "../statements/statementList.h"
#include <memory>

namespace nicole {

class NodeStructDeclaration final : public Node {
private:
  mutable std::shared_ptr<GenericType> idType_;
  mutable std::shared_ptr<NodeStatementList> body_;

public:
  NodeStructDeclaration(std::shared_ptr<GenericType> idType,
                        std::shared_ptr<NodeStatementList> body,
                        std::shared_ptr<Node> father = nullptr)
      : Node{NodeType::STRUCT_DECL, father}, idType_{idType}, body_{body} {};

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
#ifndef NODE_STRUCT_DECLARATION_H
#define NODE_STRUCT_DECLARATION_H

#include "../../types/userType.h"
#include "../node.h"
#include "../statements/statementList.h"
#include "paramsDeclaration.h"
#include <memory>

namespace nicole {

class NodeStructDeclaration final : public Node {
private:
  mutable std::shared_ptr<UserType> idType_;
  mutable std::shared_ptr<ParamsDeclaration> attributes_;

public:
  NodeStructDeclaration(std::shared_ptr<UserType> idType,
                        std::shared_ptr<ParamsDeclaration> attributes,
                        std::shared_ptr<Node> father = nullptr)
      : Node{NodeType::STRUCT_DECL, father}, idType_{idType}, attributes_{attributes} {};

  const GenericType *structType() const { return idType_.get(); }
  
  std::shared_ptr<ParamsDeclaration> attributes() const { return attributes_; }

  llvm::Value *accept(const CodeGeneration *visitor) const override {
    return visitor->visit(this);
  }

  std::string accept(const PrintTree *visitor) const override {
    return visitor->visit(this);
  }
};

} // namespace nicole

#endif
#ifndef NODE_STRUCT_DECLARATION_H
#define NODE_STRUCT_DECLARATION_H

#include "../../types/typeTable.h"
#include "../node.h"
#include "../statements/statementList.h"
#include "paramsDeclaration.h"
#include <memory>

namespace nicole {

class NodeStructDeclaration final : public Node {
private:
  mutable std::string idTypeStr_;
  mutable std::shared_ptr<ParamsDeclaration> attributes_;
  mutable std::shared_ptr<TypeTable> typeTable_;

public:
  NodeStructDeclaration(const std::string idTypeStr,
                        std::shared_ptr<ParamsDeclaration> attributes,
                        std::shared_ptr<TypeTable> typeTable,
                        std::shared_ptr<Node> father = nullptr)
      : Node{NodeType::STRUCT_DECL, father}, idTypeStr_{idTypeStr}, attributes_{attributes}, typeTable_{typeTable} {};

  std::string name() const { return idTypeStr_; }
  
  std::shared_ptr<ParamsDeclaration> attributes() const { return attributes_; }

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
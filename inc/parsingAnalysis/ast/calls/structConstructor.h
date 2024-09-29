#ifndef STRUCT_CONSTRUCTOR_H
#define STRUCT_CONSTRUCTOR_H

#include "../../types/typeTable.h"
#include "../declaration/varTable.h"
#include "../node.h"

namespace nicole {

class NodeStructConstructor : public Node {
private:
  /* data */
  std::string id_{""};
  std::shared_ptr<NodeStatementList> parameters_;
  std::shared_ptr<TypeTable> typeTable_;

public:
  NodeStructConstructor(const std::string &id,
                        std::shared_ptr<NodeStatementList> parameters,
                        std::shared_ptr<TypeTable> typeTable,
                        std::shared_ptr<Node> father = nullptr)
      : Node{NodeType::CALL_VAR, father}, id_{id}, parameters_{parameters},
        typeTable_{typeTable} {};

  std::string id() const { return id_; }

  const NodeStatementList* parameters() const { return parameters_.get(); }

  std::shared_ptr<TypeTable> table() const { return typeTable_; }

  llvm::Value *accept(const CodeGeneration *visitor) const override {
    return visitor->visit(this);
  }

  std::string accept(const PrintTree *visitor) const override {
    return visitor->visit(this);
  }
};

} // namespace nicole

#endif
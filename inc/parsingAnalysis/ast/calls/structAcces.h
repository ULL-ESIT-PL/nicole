#ifndef STRUCT_ACCES_H
#define STRUCT_ACCES_H

#include "../declaration/varTable.h"
#include "../../types/typeTable.h"
#include "../node.h"
#include <memory>

namespace nicole {

class NodeStructAcces : public Node {
private:
  /* data */
  std::string id_{""};
  std::string attribute_{""};
  std::shared_ptr<VariableTable> currentScope_;
  std::shared_ptr<TypeTable> typeTable_;

public:
  NodeStructAcces(const std::string &id, const std::string &attribute,
                  std::shared_ptr<VariableTable> currentScope,
                  std::shared_ptr<TypeTable> typeTable,
                  std::shared_ptr<Node> father = nullptr)
      : Node{NodeType::STRUCT_ACS, father}, id_{id}, attribute_{attribute},
        currentScope_{currentScope}, typeTable_{typeTable} {};

  std::string id() const { return id_; }

  std::string attribute() const { return attribute_; }

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
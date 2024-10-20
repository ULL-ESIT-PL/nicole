#ifndef STRUCT_SET_ATTR_H
#define STRUCT_SET_ATTR_H

#include "../../types/typeTable.h"
#include "../declaration/varTable.h"
#include "../node.h"
#include <memory>

namespace nicole {

class NodeStructSetAttr : public Node {
private:
  /* data */
  std::string id_{""};
  std::string attribute_{""};
  std::shared_ptr<Node> value_;
  std::shared_ptr<VariableTable> currentScope_;
  std::shared_ptr<TypeTable> typeTable_;

public:
  NodeStructSetAttr(const std::string &id, const std::string &attribute,
                    std::shared_ptr<Node> value,
                    std::shared_ptr<VariableTable> currentScope,
                    std::shared_ptr<TypeTable> typeTable,
                    std::shared_ptr<Node> father = nullptr)
      : Node{NodeType::STRUCT_SET_ATTR, father}, id_{id}, attribute_{attribute},
        value_{value}, currentScope_{currentScope}, typeTable_{typeTable} {};

  std::string id() const { return id_; }

  std::string attribute() const { return attribute_; }

  const Node* value() const { return value_.get(); }

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
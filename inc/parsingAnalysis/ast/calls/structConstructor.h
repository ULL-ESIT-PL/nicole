#ifndef STRUCT_CONSTRUCTOR_H
#define STRUCT_CONSTRUCTOR_H

#include "../../types/typeTable.h"
#include "../declaration/varTable.h"
#include "../node.h"
#include "../../types/userType.h"
#include "../literals/typedExpression.h"

namespace nicole {

class NodeStructConstructor : public Node, TypedExpression {
private:
  /* data */
  std::string id_{""};
  std::vector<std::shared_ptr<Node>> params_;
  std::shared_ptr<TypeTable> typeTable_;

public:
  NodeStructConstructor(const std::string &id,
                        std::vector<std::shared_ptr<Node>> params,
                        std::shared_ptr<TypeTable> typeTable,
                        std::shared_ptr<Node> father = nullptr)
      : Node{NodeType::CALL_CTR, father}, id_{id}, params_{params},
        typeTable_{typeTable} {};

  std::string id() const { return id_; }

  std::vector<std::shared_ptr<Node>> parameters() const { return params_; }

  std::shared_ptr<TypeTable> table() const { return typeTable_; }

  llvm::Value *accept(const CodeGeneration *visitor) const override {
    return visitor->visit(this);
  }

  std::shared_ptr<GenericType> type() const override { return typeTable_->type(id_); }

  std::string accept(const PrintTree *visitor) const override {
    return visitor->visit(this);
  }
};

} // namespace nicole

#endif
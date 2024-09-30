#ifndef FUNCTION_CALL_H
#define FUNCTION_CALL_H

#include "../declaration/functionTable.h"
#include "../declaration/varTable.h"
#include "../node.h"
#include <memory>
#include <vector>

namespace nicole {

class NodeFunctionCall : public Node {
private:
  /* data */
  std::string id_{""};
  std::vector<std::shared_ptr<Node>> params_;
  std::shared_ptr<VariableTable> variableTable_;
  std::shared_ptr<FunctionTable> functionTable_;

public:
  NodeFunctionCall(const std::string &id,
                   std::vector<std::shared_ptr<Node>> params,
                   std::shared_ptr<VariableTable> variableTable,
                   std::shared_ptr<FunctionTable> functionTable,
                   std::shared_ptr<Node> father = nullptr)
      : Node{NodeType::CALL_FUNCTION, father}, id_{id}, params_{params},
        variableTable_{variableTable}, functionTable_{functionTable} {};

  std::string id() const { return id_; }

  std::vector<std::shared_ptr<Node>> parameters() const { return params_; }

  auto begin() const { return params_.begin(); }

  auto end() const { return params_.end(); }

  std::shared_ptr<VariableTable> variableTable() const {
    return variableTable_;
  }

  std::shared_ptr<FunctionTable> functionTable() const {
    return functionTable_;
  }

  llvm::Value *accept(const CodeGeneration *visitor) const override {
    return visitor->visit(this);
  }

  std::string accept(const PrintTree *visitor) const override {
    return visitor->visit(this);
  }
};

} // namespace nicole

#endif
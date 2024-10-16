#ifndef NODE_SWITCH_STATEMENT_H
#define NODE_SWITCH_STATEMENT_H

#include "../node.h"
#include "nodeCase.h"
#include <memory>
#include <vector>

namespace nicole {

class NodeSwitchStatement final : public Node {
private:
  mutable std::shared_ptr<Node> match_;
  mutable std::vector<std::shared_ptr<NodeCaseStatement>> cases_;

public:
  NodeSwitchStatement(std::shared_ptr<Node> match,
                      std::vector<std::shared_ptr<NodeCaseStatement>> cases,
                      std::shared_ptr<Node> father = nullptr)
      : Node{NodeType::SWITCH, father}, match_{match}, cases_{cases} {};

  const Node *match() const { return match_.get(); }

  std::vector<std::shared_ptr<NodeCaseStatement>> cases() const { return cases_; }

  auto begin() const { return cases_.begin(); }

  auto end() const { return cases_.end(); }

  llvm::Value *accept(const CodeGeneration *visitor) const override {
    return visitor->visit(this);
  }

  std::string accept(const PrintTree *visitor) const override {
    return visitor->visit(this);
  }
};

} // namespace nicole

#endif
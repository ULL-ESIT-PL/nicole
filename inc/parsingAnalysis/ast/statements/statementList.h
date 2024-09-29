#ifndef NODE_STATEMENT_LIST_H
#define NODE_STATEMENT_LIST_H

#include <memory>
#include <vector>

#include "statement.h"

namespace nicole {

class NodeStatementList final : public Node {
private:
  std::vector<std::shared_ptr<NodeStatement>> statements_{};

public:
  NodeStatementList(const std::vector<std::shared_ptr<NodeStatement>> &statements,
                    std::shared_ptr<Node> father = nullptr)
      : Node{NodeType::STATEMENT_LIST, father}, statements_(statements) {
    // No need to move the elements individually anymore, as std::move handles
    // it
  }

  ~NodeStatementList() = default;

  const std::vector<std::shared_ptr<NodeStatement>> &statements() const {
    return statements_;
  }

  void addStatement(std::shared_ptr<NodeStatement> statement) {
    statements_.push_back(statement);
  }

  auto begin() const { return statements_.begin(); }

  auto end() const { return statements_.end(); }

  llvm::Value *accept(const CodeGeneration *visitor) const override {
    return visitor->visit(this);
  }

  std::string accept(const PrintTree *visitor) const override {
    return visitor->visit(this);
  }
};

} // namespace nicole

#endif
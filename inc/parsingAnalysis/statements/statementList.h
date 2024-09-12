#ifndef NODE_STATEMENT_LIST_H
#define NODE_STATEMENT_LIST_H

#include <memory>
#include <vector>

#include "statement.h"
using namespace std;

namespace nicole {
class NodeStatementList final : public Node {
 private:
  std::vector<std::unique_ptr<NodeStatement>> statements_{};

 public:
  NodeStatementList(const std::vector<std::unique_ptr<NodeStatement>>& statements,
                std::unique_ptr<Node> father = nullptr)
      : Node{NodeType::STATEMENT, std::move(father)},
        statements_{statements} {};

  ~NodeStatementList() = default;

  std::vector<std::unique_ptr<NodeStatement>> statements() const { return statements_; }

  void addStatement(std::unique_ptr<NodeStatement> statement) {
    statements_.push_back(statement);
  }

  auto begin() const { return statements_.begin(); }

  auto end() const { return statements_.end(); }

  llvm::Value* accept(const Visitor* visitor) const override {
    return visitor->visit(this);
  }
};

}  // namespace nicole

#endif
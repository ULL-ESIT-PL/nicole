#ifndef NODE_IF_STATEMENT_H
#define NODE_IF_STATEMENT_H

#include "../node.h"
#include "../statements/statementList.h"

namespace nicole {

class NodeIfStatement final : public Node {
private:
  mutable std::unique_ptr<Node> condition_;
  mutable std::unique_ptr<NodeStatementList> body_;
  mutable std::unique_ptr<NodeStatementList> elseBody_;

public:
  NodeIfStatement(std::unique_ptr<Node> condition,
                  std::unique_ptr<NodeStatementList> body,
                  std::unique_ptr<NodeStatementList> elseBody = nullptr,
                  std::unique_ptr<Node> father = nullptr)
      : Node{NodeType::IF, std::move(father)}, condition_{std::move(condition)},
        body_{std::move(body)}, elseBody_{std::move(elseBody)} {};

  std::unique_ptr<Node> condition() const { return std::move(condition_); }

  std::unique_ptr<NodeStatementList> body() const { return std::move(body_); }

  bool hasElse() const { return elseBody_ != nullptr; }

  std::unique_ptr<NodeStatementList> elseBody() const {
    return std::move(elseBody_);
  }

  llvm::Value *accept(const Visitor *visitor, llvm::BasicBlock *currentEntry,
                      llvm::Module *currentModule) const override {
    return visitor->visit(this, currentEntry, currentModule);
  }
};

} // namespace nicole

#endif
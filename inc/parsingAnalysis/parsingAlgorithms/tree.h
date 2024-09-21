#ifndef TREE_H
#define TREE_H

#include "../ast/statements/statementList.h"

namespace nicole {

class Tree {
private:
  /* data */
  std::unique_ptr<NodeStatementList> root_{};

public:
  Tree(std::unique_ptr<NodeStatementList> root) : root_{std::move(root)} {};

  NodeStatementList *root() const { return root_.get(); }

  llvm::Value *accept(std::unique_ptr<CodeGeneration> visitor) const {
    return visitor->visit(this);
  }

  std::string accept(std::unique_ptr<PrintTree> visitor) const {
    return visitor->visit(this);
  }
};

} // namespace nicole

#endif
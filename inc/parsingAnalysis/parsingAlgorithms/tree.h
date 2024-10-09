#ifndef TREE_H
#define TREE_H

#include "../ast/statements/statementList.h"

namespace nicole {

// AST of our language
class Tree {
private:
  /* data */
  std::shared_ptr<NodeStatementList> root_{};

public:
  Tree(std::shared_ptr<NodeStatementList> root) : root_{root} {};

  const NodeStatementList *root() const { return root_.get(); }

  llvm::Value *accept(std::unique_ptr<CodeGeneration> visitor) const {
    return visitor->visit(this);
  }

  std::string accept(std::unique_ptr<PrintTree> visitor) const {
    return visitor->visit(this);
  }
};

} // namespace nicole

#endif
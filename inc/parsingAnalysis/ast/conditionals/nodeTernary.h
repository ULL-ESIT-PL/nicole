#ifndef NODE_TERNARY_H
#define NODE_TERNARY_H

#include "../node.h"
#include <memory>

namespace nicole {

class NodeTernary final : public Node {
private:
  mutable std::shared_ptr<Node> condition_;
  mutable std::shared_ptr<Node> first_;
  mutable std::shared_ptr<Node> second_;

public:
  NodeTernary(std::shared_ptr<Node> condition, std::shared_ptr<Node> first,
              std::shared_ptr<Node> second,
              std::shared_ptr<Node> father = nullptr)
      : Node{NodeType::TERNARY, father}, condition_{condition}, first_{first},
        second_{second} {};

  const Node *condition() const { return condition_.get(); }

  const Node *first() const { return first_.get(); }

  const Node *second() const { return second_.get(); }

  llvm::Value *accept(const CodeGeneration *visitor) const override {
    return visitor->visit(this);
  }

  std::string accept(const PrintTree *visitor) const override {
    return visitor->visit(this);
  }
};

} // namespace nicole

#endif
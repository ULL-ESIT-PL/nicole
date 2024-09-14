#ifndef VARIABLE_DECLARATION
#define VARIABLE_DECLARATION

#include "../node.h"

namespace nicole {

class NodeVariableDeclaration : public Node {
 private:
  /* data */
  std::string id_{""};
  
  std::unique_ptr<Node> expression_;

 public:
  NodeVariableDeclaration(const std::string& id,
                          std::unique_ptr<Node> expression,
                          std::unique_ptr<Node> father = nullptr)
      : Node{NodeType::VAR_DECL, std::move(father)},
        id_{id},
        expression_{std::move(expression)} {};

  std::string id() const { return id_; }

  Node* expression() const { return expression_.get(); }

  llvm::Value* accept(const Visitor* visitor) const override {
    return visitor->visit(this);
  }
};

}  // namespace nicole

#endif
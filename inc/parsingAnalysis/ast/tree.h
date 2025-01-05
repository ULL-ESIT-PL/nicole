#ifndef TREE_H
#define TREE_H

#include "statements/ast_body.h"
#include <memory>

namespace nicole {

class Tree final {
private:
  std::shared_ptr<AST_BODY> statements_;

public:
  explicit Tree(const std::shared_ptr<AST_BODY> &statements) noexcept
      : statements_{statements} {}

  [[nodiscard]] const std::shared_ptr<AST_BODY> &statements() const noexcept {
    return statements_;
  }
};

} // namespace nicole

#endif
#ifndef CHECK_POSITION_H
#define CHECK_POSITION_H

#include "ast/ast.h"
#include <unordered_set>

namespace nicole {

class CheckPosition final {
private:
  ~CheckPosition() = delete;

public:
  [[nodiscard]] static bool
  hasAnyAncestorOf(const std::shared_ptr<AST> &node,
          const std::unordered_set<AST_TYPE> &possibles) noexcept;

  [[nodiscard]] static bool
  hasEveryAncestorInOrder(const std::shared_ptr<AST> &node,
          const std::unordered_set<AST_TYPE> &possibles) noexcept;

  [[nodiscard]] static bool
  itsBodyAncestorHasParent(const std::shared_ptr<AST> &node,
          const std::unordered_set<AST_TYPE> &possibles) noexcept;

  [[nodiscard]] static bool
  isInsideForHeader(const std::shared_ptr<AST> &node,
          const std::unordered_set<AST_TYPE> &possibles) noexcept;

  [[nodiscard]] static bool
  hasLoopAncestor(const std::shared_ptr<AST> &node,
          const std::unordered_set<AST_TYPE> &possibles) noexcept;
};

} // namespace nicole

#endif
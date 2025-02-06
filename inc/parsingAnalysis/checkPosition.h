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
  hasAnyAncestorOf(const AST* node,
                   const std::unordered_set<AST_TYPE> &possibles) noexcept;

  [[nodiscard]] static bool
  hasEveryAncestorInOrder(const AST* node,
                          const std::vector<AST_TYPE> &possibles) noexcept;

  [[nodiscard]] static bool
  itsBodyAncestorHasParent(const AST* node) noexcept;

  [[nodiscard]] static bool
  isInsideForHeader(const AST* node) noexcept;

  [[nodiscard]] static bool
  hasLoopAncestor(const AST* node) noexcept;

  [[nodiscard]] static bool
  hasAssigmentOrDeclAncestor(const AST* node) noexcept;
};

} // namespace nicole

#endif
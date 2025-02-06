#include "../../inc/parsingAnalysis/checkPosition.h"
#include <cstddef>
#include <iostream>

namespace nicole {

bool CheckPosition::hasAnyAncestorOf(
    const AST *node, const std::unordered_set<AST_TYPE> &possibles) noexcept {
  auto auxiliar{node};
  while (auxiliar->father()) {
    auxiliar = auxiliar->father().get();
    if (possibles.count(auxiliar->type())) {
      return true;
    }
  }
  return false;
}

bool CheckPosition::hasEveryAncestorInOrder(
    const AST *node, const std::vector<AST_TYPE> &possibles) noexcept {
  auto auxiliar{node};
  size_t index{0};
  while (auxiliar->father() and index < possibles.size()) {
    auxiliar = auxiliar->father().get();
    if (auxiliar->type() != possibles[index]) {
      return false;
    }
    ++index;
  }
  return true;
}

bool CheckPosition::itsBodyAncestorHasParent(const AST *node) noexcept {
  std::shared_ptr<AST> father{nullptr};
  if (!node->father()) {
    return false;
  }
  father = node->father();
  if (father->type() != AST_TYPE::STATEMENT) {
    return false;
  }
  if (!father->father()) {
    return false;
  }
  father = father->father();
  if (father->type() != AST_TYPE::BODY) {
    return false;
  }
  if (!father->father()) {
    return false;
  }
  return true;
}

bool CheckPosition::isInsideForHeader(const AST *node) noexcept {
  return (node->father() and node->father()->type() == AST_TYPE::FOR) ? true
                                                                      : false;
}

bool CheckPosition::hasLoopAncestor(const AST *node) noexcept {
  return hasAnyAncestorOf(node,
                          {AST_TYPE::WHILE, AST_TYPE::FOR, AST_TYPE::DO_WHILE});
}

bool CheckPosition::hasAssigmentOrDeclAncestor(const AST *node) noexcept {
  return hasAnyAncestorOf(node, {AST_TYPE::CONST_DECL, AST_TYPE::AUTO_DECL,
                                 AST_TYPE::LET_DECL, AST_TYPE::ASIGNMENT,
                                 AST_TYPE::SELF_ADD, AST_TYPE::SELF_SUB,
                                 AST_TYPE::SELF_MULT, AST_TYPE::SELF_DIV});
}

bool CheckPosition::isOutOfScope(const AST *node) noexcept {
  auto auxiliar{node};
  while (auxiliar->father()) {
    auxiliar = auxiliar->father().get();
    if (auxiliar->type() == AST_TYPE::BODY and auxiliar->father()) {
      return false;
    }
  }
  return true;
}

} // namespace nicole
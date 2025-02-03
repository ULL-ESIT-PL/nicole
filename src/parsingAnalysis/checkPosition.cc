#include "../../inc/parsingAnalysis/checkPosition.h"

namespace nicole {

bool CheckPosition::hasAnyAncestorOf(
    const std::shared_ptr<AST> &node,
    const std::unordered_set<AST_TYPE> &possibles) noexcept {}

bool CheckPosition::hasEveryAncestorInOrder(
    const std::shared_ptr<AST> &node,
    const std::unordered_set<AST_TYPE> &possibles) noexcept {}

bool CheckPosition::itsBodyAncestorHasParent(
    const std::shared_ptr<AST> &node,
    const std::unordered_set<AST_TYPE> &possibles) noexcept {}

bool CheckPosition::isInsideForHeader(
    const std::shared_ptr<AST> &node,
    const std::unordered_set<AST_TYPE> &possibles) noexcept {}

bool CheckPosition::hasLoopAncestor(
    const std::shared_ptr<AST> &node,
    const std::unordered_set<AST_TYPE> &possibles) noexcept {}

} // namespace nicole
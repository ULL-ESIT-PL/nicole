#include "../../inc/parsingAnalysis/checkPosition.h"

namespace nicole {

bool CheckPosition::isValid(
    const std::shared_ptr<AST> &node,
    const std::unordered_set<AST_TYPE> &possibles) noexcept {
  auto aux{node};
  while (aux->father()) {
    if (possibles.count(aux->father()->type())) {
        return true;
    }
    aux = aux->father();
  }
  return false;
}

} // namespace nicole
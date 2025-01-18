#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"

namespace nicole {

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseVarDecl() const noexcept {
  return parseTernary();
}

} // namespace nicole
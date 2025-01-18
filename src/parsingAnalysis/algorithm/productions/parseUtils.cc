#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"

namespace nicole {

const std::expected<std::shared_ptr<AST_IMPORT>, Error>
TopDown::parseImport() const noexcept {
  return nullptr;
}

const std::expected<std::shared_ptr<AST_PRINT>, Error>
TopDown::parsePrint() const noexcept {
  return nullptr;
}

} // namespace nicole
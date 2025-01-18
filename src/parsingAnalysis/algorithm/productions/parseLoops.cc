#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"

namespace nicole {

const std::expected<std::shared_ptr<AST_WHILE>, Error>
TopDown::parseWhile() const noexcept {
  return nullptr;
}

const std::expected<std::shared_ptr<AST_FOR>, Error>
TopDown::parseFor() const noexcept {
  return nullptr;
}

const std::expected<std::shared_ptr<AST_DO_WHILE>, Error>
TopDown::parseDoWhile() const noexcept {
  return nullptr;
}

} // namespace nicole
#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"

namespace nicole {

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseOr() const noexcept {
  return nullptr;
}

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseEqual_NotEqual() const noexcept {
  return nullptr;
}

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseCompare() const noexcept {
  return nullptr;
}

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseAdd_Sub() const noexcept {
  return nullptr;
}

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseMult_Div_Module() const noexcept {
  return nullptr;
}

} // namespace nicole
#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"

namespace nicole {

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseTernary() const noexcept {}

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseOr() const noexcept {}

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseAnd() const noexcept {}

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseEqual_NotEqual() const noexcept {}

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseCompare() const noexcept {}

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseAdd_Sub() const noexcept {}

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseMult_Div_Module() const noexcept {}

} // namespace nicole
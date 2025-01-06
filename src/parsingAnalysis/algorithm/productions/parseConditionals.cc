#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"

namespace nicole {

const std::expected<std::shared_ptr<AST_IF>, Error>
TopDown::parseIf() const noexcept {}

const std::expected<std::shared_ptr<AST_ELSE_IF>, Error>
TopDown::parseElseIf() const noexcept {}

const std::expected<std::shared_ptr<AST_SWITCH>, Error>
TopDown::parseSwitch() const noexcept {}

const std::expected<std::shared_ptr<AST_CASE>, Error>
TopDown::parseCase() const noexcept {}

const std::expected<std::shared_ptr<AST_DEFAULT>, Error>
TopDown::parseDefault() const noexcept {}

} // namespace nicole
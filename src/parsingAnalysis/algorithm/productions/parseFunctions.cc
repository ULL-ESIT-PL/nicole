#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"

namespace nicole {

const std::expected<std::shared_ptr<AST_FUNC_DECL>, Error>
TopDown::parseFuncDecl() const noexcept { return nullptr; }

const std::expected<std::shared_ptr<AST_RETURN>, Error>
TopDown::parseReturn() const noexcept { return nullptr; }

} // namespace nicole
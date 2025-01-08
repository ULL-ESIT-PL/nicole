#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"

namespace nicole {

const std::expected<std::shared_ptr<AST_ENUM>, Error>
TopDown::parseEnum() const noexcept{ return nullptr; }

const std::expected<std::shared_ptr<AST_CLASS>, Error>
TopDown::parseClassDecl() const noexcept{ return nullptr; }

const std::expected<std::shared_ptr<AST_STRUCT>, Error>
TopDown::parseStructDecl() const noexcept{ return nullptr; }

} // namespace nicole
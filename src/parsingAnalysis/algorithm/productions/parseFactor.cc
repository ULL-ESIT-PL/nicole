#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"

namespace nicole {

const std::expected<std::shared_ptr<AST>, Error>
TopDown::parseFactor() const noexcept {}

const std::expected<std::shared_ptr<AST_VECTOR>, Error>
TopDown::parseVector() const noexcept {}

const std::expected<Parameters, Error> TopDown::parseParams() const noexcept {}

const std::expected<std::vector<std::shared_ptr<AST>>, Error>
TopDown::parseArguments() const noexcept {}

} // namespace nicole
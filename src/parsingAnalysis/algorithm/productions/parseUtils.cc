#include "../../../../inc/parsingAnalysis/algorithm/topDown.h"

namespace nicole {

const std::expected<std::shared_ptr<AST_IMPORT>, Error>
TopDown::parseImport() const noexcept {}

const std::expected<std::shared_ptr<AST_PRINT>, Error>
TopDown::parsePrint() const noexcept {
    
}

} // namespace nicole
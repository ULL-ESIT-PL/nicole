#include "../../../inc/parsingAnalysis/algorithm/topDown.h"
#include <memory>

namespace nicole {

const std::expected<std::shared_ptr<Tree>, Error>
TopDown::parse(const std::filesystem::path &entryFile) const noexcept {
  if (entryFile.empty()) {
  }
  return std::make_shared<Tree>(*Builder::createBody({}, SourceLocation{Location{"", 0, 0}, Location{"", 0, 0}}));
}

} // namespace nicole
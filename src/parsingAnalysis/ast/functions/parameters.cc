#include "../../../../inc/parsingAnalysis/ast/functions/parameters.h"

namespace nicole {

bool Parameters::operator==(const Parameters &other) const noexcept {
  if (params_.size() != other.size()) {
    return false;
  }
  const std::vector<std::pair<std::string, std::string>> data{other.params_};
  const std::size_t size{params_.size()};
  for (std::size_t i{0}; i < size; ++i) {
    if (params_[i].second != data[i].second) {
      return false;
    }
  }
  return true;
}

} // namespace nicole
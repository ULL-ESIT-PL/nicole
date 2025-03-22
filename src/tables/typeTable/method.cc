#include "../../../inc/tables/typeTable/types/userTypes/method.h"

namespace nicole {

bool Method::operator==(const Method &other) const noexcept {
  if (id_ != other.id_) {
    return false;
  }
  if (params_.size() != other.params_.size()) {
    return false;
  }
  const auto parameters{params_.params()};
  const auto parametersOther{other.params_.params()};
  for (size_t i{0}; i < parameters.size(); ++i) {
    if (parameters[i].second->toString() !=
        parametersOther[i].second->toString()) {
      return false;
    }
  }
  return true;
}

} // namespace nicole
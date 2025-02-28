#ifndef GENERIC_PARAMETER_H
#define GENERIC_PARAMETER_H

#include <string>

namespace nicole {

class GenericParameter final {
  std::string name_;

public:
  explicit GenericParameter(const std::string& name) noexcept : name_{name} {}

  [[nodiscard]] const std::string& name() const noexcept { return name_; }
};

} // namespace nicole

#endif
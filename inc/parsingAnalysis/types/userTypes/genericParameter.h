#ifndef GENERIC_PARAMETER_H
#define GENERIC_PARAMETER_H

#include <string>

namespace nicole {

class GenericParameter {
  std::string name;

public:
  explicit GenericParameter(std::string n) : name(std::move(n)) {}

  [[nodiscard]] std::string toString() const noexcept { return name; }
};

} // namespace nicole

#endif
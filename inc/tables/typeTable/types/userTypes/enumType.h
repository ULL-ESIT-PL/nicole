#ifndef ENUM_TYPE_H
#define ENUM_TYPE_H

#include "../type.h"
#include <sstream>
#include <string>
#include <vector>

namespace nicole {

class EnumType final : public Type {
private:
  std::string name_;
  std::vector<std::string> values_;

public:
  EnumType(const std::string &name,
           const std::vector<std::string> &values) noexcept
      : name_(name), values_(values) {}

  [[nodiscard]] const std::string &name() const noexcept { return name_; }

  [[nodiscard]] const std::vector<std::string> &values() const noexcept {
    return values_;
  }

  [[nodiscard]] std::string toString() const noexcept override {
    std::ostringstream oss;
    oss << "enum " << name_ << " { ";
    for (size_t i = 0; i < values_.size(); ++i) {
      oss << values_[i];
      if (i != values_.size() - 1) {
        oss << ", ";
      }
    }
    oss << " }";
    return oss.str();
  }
};

} // namespace nicole

#endif

#ifndef GENERIC_PARAMETER_H
#define GENERIC_PARAMETER_H

#include <string>
#include <functional>

namespace nicole {

class GenericParameter final {
private:
  std::string name_;

public:
  explicit GenericParameter(const std::string &name) noexcept : name_{name} {}

  [[nodiscard]] const std::string &name() const noexcept { return name_; }

  [[nodiscard]] bool operator==(const GenericParameter &other) const noexcept {
    return name_ == other.name_;
  }

  [[nodiscard]] bool operator<(const GenericParameter &other) const noexcept {
    return name_ < other.name_;
  }
};

} // namespace nicole

namespace std {
    template<>
    struct hash<nicole::GenericParameter> {
        std::size_t operator()(const nicole::GenericParameter& obj) const noexcept {
            return std::hash<std::string>{}(obj.name());
        }
    };
}

#endif
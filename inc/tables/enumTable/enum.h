#ifndef ENUM_H
#define ENUM_H

#include <string>
#include <vector>

namespace nicole {

class Enum final {
private:
  std::string id_;
  std::vector<std::string> identifiers_;

public:
  explicit Enum(const std::string &id,
                const std::vector<std::string> &identifiers) noexcept
      : id_{id}, identifiers_{identifiers} {}

  [[nodiscard]] const std::string &id() const noexcept { return id_; }

  [[nodiscard]] const std::vector<std::string> &identifiers() const noexcept {
    return identifiers_;
  }

  [[nodiscard]] auto begin() const noexcept { return identifiers_.begin(); }

  [[nodiscard]] auto end() const noexcept { return identifiers_.end(); }
};

} // namespace nicole

#endif
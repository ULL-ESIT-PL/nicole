#ifndef ENUM_H
#define ENUM_H

#include "../symbol.h"
#include <string>
#include <vector>

namespace nicole {

class Enum final : public Symbol {
private:
  std::vector<std::string> identifiers_;

public:
  explicit Enum(const std::string &id,
                const std::vector<std::string> &identifiers) noexcept
      : Symbol{id}, identifiers_{identifiers} {}

  [[nodiscard]] const std::vector<std::string> &identifiers() const noexcept {
    return identifiers_;
  }

  [[nodiscard]] auto begin() const noexcept { return identifiers_.begin(); }

  [[nodiscard]] auto end() const noexcept { return identifiers_.end(); }
};

} // namespace nicole

#endif
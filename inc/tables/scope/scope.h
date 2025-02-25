#ifndef SCOPE_H
#define SCOPE_H

#include "../../errors.h"
#include "variable.h"
#include <expected>
#include <memory>
#include <string>
#include <unordered_map>

namespace nicole {

class Scope final {
private:
  std::unordered_map<std::string, Variable> table_;
  std::shared_ptr<Scope> father_;

public:
  explicit Scope(const std::shared_ptr<Scope> &father) noexcept
      : father_{father} {}

  [[nodiscard]] const std::shared_ptr<Scope> &father() const noexcept {
    return father_;
  }

  [[nodiscard]] bool has(const std::string &id) const noexcept;

  [[nodiscard]] const Variable &
  getVariable(const std::string &id) const noexcept;

  void insert(const Variable &variable) noexcept;
};

} // namespace nicole

#endif
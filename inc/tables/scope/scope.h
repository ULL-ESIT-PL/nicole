#ifndef SCOPE_H
#define SCOPE_H

#include "../../errors.h"
#include "variable.h"
#include <expected>
#include <iostream>
#include <memory>
#include <ostream>
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

  [[nodiscard]] const std::expected<Variable, Error>
  getVariable(const std::string &id) const noexcept;

  [[nodiscard]] std::expected<std::monostate, Error>
  insert(const Variable &variable) noexcept;

  friend std::ostream &operator<<(std::ostream &os,
                                  const Scope &scope) noexcept;

}; // namespace nicole

}

#endif
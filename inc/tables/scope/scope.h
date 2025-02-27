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

  [[nodiscard]] bool has(const std::string &id) const noexcept {
    if (table_.count(id)) {
      return true;
    }
    if (father_) {
      return father_->has(id);
    }
    return false;
  };

  [[nodiscard]] const std::expected<Variable, Error>
  getVariable(const std::string &id) const noexcept {
    if (table_.count(id)) {
      return table_.at(id);
    }
    if (father_) {
      return father_->getVariable(id);
    }
    return createError(ERROR_TYPE::VARIABLE,
                       "variable: " + id + " does not exist");
  };

  void insert(const Variable &variable, const bool forceInsert) noexcept {
    if (forceInsert or !has(variable.id())) {
      table_.emplace(variable.id(), variable);
    }
  };

  friend std::ostream &operator<<(std::ostream &os,
                                  const Scope &scope) noexcept {
                                    std::cout << "hola" << std::flush;
    os << "Scope { ";
    for (auto it = scope.table_.cbegin(); it != scope.table_.cend(); ++it) {
      os << it->first;
      if (std::next(it) != scope.table_.cend()) {
        os << ", ";
      }
    }
    os << " }";
    if (scope.father_) {
      os << " -> " << *scope.father_;
    }
    return os;
  }
};

} // namespace nicole

#endif
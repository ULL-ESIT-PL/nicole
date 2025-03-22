#ifndef METHOD_TABLE_H
#define METHOD_TABLE_H

#include "../../../../errors.h"
#include "method.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace nicole {

class MethodTable {
private:
  std::unordered_map<std::string, std::vector<Method>> table_;

public:
  [[nodiscard]] bool has(const Method &method) const noexcept;

  [[nodiscard]] bool hasSymbol(const std::string& id) const noexcept {
    return table_.count(id);
  };

  [[nodiscard]] const std::expected<Method, Error>
  getMethod(const std::string &id, const Parameters &params) const noexcept;

  [[nodiscard]] const std::expected<std::vector<Method>, Error>
  getMethods(const std::string &id) const noexcept;

  [[nodiscard]] std::expected<std::monostate, Error>
  insert(const Method &method) noexcept;

  void print() const noexcept;
};

} // namespace nicole

#endif
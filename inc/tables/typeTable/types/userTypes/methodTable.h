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
  [[nodiscard]] std::vector<Method>
  getMethods(const std::string &id) const noexcept;

  void insert(const Method &method) noexcept;

  void print() const noexcept;
};

} // namespace nicole

#endif
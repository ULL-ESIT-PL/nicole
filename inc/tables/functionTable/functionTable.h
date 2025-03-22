#ifndef FUNCTION_TABLE_H
#define FUNCTION_TABLE_H

#include "function.h"
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace nicole {

class FunctionTable {
private:
  std::unordered_map<std::string, std::vector<Function>> table_;

public:
  [[nodiscard]] std::vector<Function>
  getFunctions(const std::string &id) const noexcept;

  void insert(const Function &function) noexcept;

  void print() const noexcept;
};

} // namespace nicole

#endif
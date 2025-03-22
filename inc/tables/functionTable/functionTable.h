#ifndef FUNCTION_TABLE_H
#define FUNCTION_TABLE_H

#include "../typeTable/typeTable.h"
#include "function.h"
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace nicole {

class FunctionTable {
private:
  std::shared_ptr<TypeTable> typeTable_;
  std::unordered_map<std::string, std::vector<Function>> table_;

public:
  explicit FunctionTable(const std::shared_ptr<TypeTable> &typeTable) noexcept
      : typeTable_{typeTable} {}

  [[nodiscard]] std::vector<Function>
  getFunctions(const std::string &id) const noexcept;

  void insert(const Function &function) noexcept;

  void print() const noexcept;
};

} // namespace nicole

#endif
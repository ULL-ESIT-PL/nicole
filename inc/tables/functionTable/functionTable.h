#ifndef FUNCTION_TABLE_H
#define FUNCTION_TABLE_H

#include "function.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace nicole {

class FunctionTable {
private:
  std::unordered_map<std::string, std::vector<Function>> table_;

public:
  [[nodiscard]] bool has(const std::string &id,
                         const std::vector<GenericParameter> &generics,
                         const Parameters &params) const noexcept;

  [[nodiscard]] const std::vector<Function> &
  getFunction(const std::string &id) const noexcept;

  void insert(const Function &function) noexcept;
};

} // namespace nicole

#endif
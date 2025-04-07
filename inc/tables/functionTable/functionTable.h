#ifndef FUNCTION_TABLE_H
#define FUNCTION_TABLE_H

#include "../../errors.h"
#include "function.h"
#include <expected>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
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

  [[nodiscard]] std::expected<std::monostate, Error>
  setFuncReturnType(const std::string &id,
                    const std::shared_ptr<Type> &type) noexcept;

  [[nodiscard]] std::expected<std::monostate, Error>
  setFuncParameters(const std::string &id,
                    const Parameters &params) noexcept;
};

} // namespace nicole

#endif
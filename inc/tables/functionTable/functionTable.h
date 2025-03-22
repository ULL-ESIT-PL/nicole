#ifndef FUNCTION_TABLE_H
#define FUNCTION_TABLE_H

#include "../../errors.h"
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

  [[nodiscard]] bool has(const Function &function) const noexcept;

  [[nodiscard]] const std::expected<Function, Error>
  getFunction(const std::string &id, const Parameters &params) const noexcept;

  [[nodiscard]] const std::expected<std::vector<Function>, Error>
  getFunctions(const std::string &id) const noexcept;

  [[nodiscard]] std::expected<std::monostate, Error>
  insert(const Function &function) noexcept;

  [[nodiscard]] bool areAmbiguous(const Function &first,
                                  const Function &second) const noexcept;

  void print() const noexcept;
};

} // namespace nicole

#endif
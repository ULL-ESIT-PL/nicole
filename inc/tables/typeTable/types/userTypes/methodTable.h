#ifndef METHOD_TABLE_H
#define METHOD_TABLE_H

#include "../../../../errors.h"
#include "genericParameter.h"
#include "method.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace nicole {

class MethodTable {
private:
  mutable std::unordered_map<std::string, std::vector<Method>> table_;

public:
  [[nodiscard]] std::vector<Method>
  getMethods(const std::string &id) const noexcept;

  void insert(const Method &method) noexcept;

  void print() const noexcept;

  [[nodiscard]] std::expected<std::monostate, Error>
  setMethodReturnType(const std::string &id,
                      const std::shared_ptr<Type> &type) noexcept;

  [[nodiscard]] std::expected<std::monostate, Error>
  setMethodParameters(const std::string &id, const Parameters &params) noexcept;

  [[nodiscard]] std::expected<std::monostate, Error>
  setMethodGenercis(const std::string &id,
                    const std::vector<GenericParameter> &generics) noexcept;
};

} // namespace nicole

#endif
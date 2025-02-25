#ifndef ENUM_TABLE_H
#define ENUM_TABLE_H

#include "enum.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace nicole {

class EnumTable {
private:
  std::unordered_map<std::string, Enum> table_;

public:
  [[nodiscard]] bool has(const std::string &id) const noexcept {
    return table_.count(id);
  };

  [[nodiscard]] const Enum &getEnum(const std::string &id) const noexcept {
    return table_.at(id);
  };

  void insert(const Enum &enum_) noexcept;
};

} // namespace nicole

#endif
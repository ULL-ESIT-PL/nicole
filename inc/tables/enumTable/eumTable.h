#ifndef ENUM_TABLE_H
#define ENUM_TABLE_H

#include "../../errors.h"
#include "enum.h"
#include <expected>
#include <string>
#include <unordered_map>

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

  [[nodiscard]] std::expected<std::monostate, Error>
  insert(const Enum &enum_) noexcept {
    if (!table_.count(enum_.id())) {
      table_.emplace(enum_.id(), enum_);
      return {};
    }
    return createError(ERROR_TYPE::ENUM,
                       "the enum: " + enum_.id() + " already exists");
  }
};

} // namespace nicole

#endif
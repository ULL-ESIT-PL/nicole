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

  [[nodiscard]] const std::expected<Enum, Error> getEnum(const std::string &id) const noexcept {
    if (!has(id)) {
      return createError(ERROR_TYPE::ENUM,
                         "the enum: " + id + " does not exists");
    }
    return table_.at(id);
  };

  [[nodiscard]] std::expected<std::monostate, Error>
  insert(const Enum &enum_) noexcept {
    if (table_.count(enum_.id())) {
      return createError(ERROR_TYPE::ENUM,
                         "the enum: " + enum_.id() + " already exists");
    }
    table_.emplace(enum_.id(), enum_);
    return {};
  }
};

} // namespace nicole

#endif
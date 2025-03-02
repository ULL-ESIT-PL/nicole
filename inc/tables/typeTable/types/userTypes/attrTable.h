#ifndef ATTR_TABLE_H
#define ATTR_TABLE_H

#include "../../../../errors.h"
#include "attribute.h"
#include <expected>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>

namespace nicole {

class AttrTable final {
private:
  std::unordered_map<std::string, Attribute> table_;

public:
  [[nodiscard]] bool has(const std::string &id) const noexcept;

  [[nodiscard]] const std::expected<Attribute, Error>
  getAttribute(const std::string &id) const noexcept;

  [[nodiscard]] std::expected<std::monostate, Error>
  insert(const Attribute &variable) noexcept;
}; // namespace nicole

}

#endif
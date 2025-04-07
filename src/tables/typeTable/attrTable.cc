#include "../../../inc/tables/typeTable/types/userTypes/attrTable.h"

namespace nicole {

bool AttrTable::has(const std::string &id) const noexcept {
  return table_.count(id);
}

const std::expected<Attribute, Error>
AttrTable::getAttribute(const std::string &id) const noexcept {
  if (table_.count(id)) {
    return table_.at(id);
  }
  return createError(ERROR_TYPE::ATTR, "Attribute: " + id + " does not exist");
}

std::expected<std::monostate, Error>
AttrTable::insert(const Attribute &attribute) noexcept {
  if (!has(attribute.id())) {
    table_.emplace(attribute.id(), attribute);
    return {};
  }
  return createError(ERROR_TYPE::ATTR,
                     "the attribute: " + attribute.id() + " already exists");
}

std::expected<std::monostate, Error>
AttrTable::setAttribute(const Attribute &attribute) noexcept {
  if (!has(attribute.id())) {
    return createError(ERROR_TYPE::ATTR,
                       "Attribute: " + attribute.id() + " does not exist");
  }
  table_.at(attribute.id()) = attribute;
  return {};
}


} // namespace nicole
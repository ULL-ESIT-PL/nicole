#include "../../../inc/tables/typeTable/typeTable.h"

namespace nicole {

bool TypeTable::has(const std::string &id) const noexcept {
  return table_.count(id);
}

const std::expected<std::shared_ptr<Type>, Error>
TypeTable::getType(const std::string &id) const noexcept {
  if (has(id)) {
    return table_.at(id);
  }
  return createError(ERROR_TYPE::FUNCTION,
                     "the type: " + id + " does not exists");
}

std::expected<std::monostate, Error>
TypeTable::insert(const std::shared_ptr<Type> &type) noexcept {
  if (has(type->toString())) {
    return createError(ERROR_TYPE::FUNCTION,
                       "the type: " + type->toString() + " already exists");
  }
  table_[type->toString()] = type;
  return {};
}

} // namespace nicole
#include "../../../inc/tables/typeTable/types/userTypes/methodTable.h"

namespace nicole {

bool MethodTable::has(const Method &method) const noexcept {
  if (!table_.count(method.id())) {
    return false;
  }
  const auto vec{table_.at(method.id())};
  for (const auto &func : vec) {
    if (func == method) {
      return true;
    }
  }
  return false;
}

const std::expected<Method, Error>
MethodTable::getMethod(const std::string &id,
                           const Parameters &params) const noexcept {
  if (!table_.count(id)) {
    return createError(ERROR_TYPE::METHOD,
                       "the method with id: " + id + " does not exist");
  }
  const Method aux{id, {}, params, nullptr};
  for (const auto &func : table_.at(id)) {
    if (func == aux) { // Se asume que Parameters tiene operator==
      return func;
    }
  }
  return createError(ERROR_TYPE::METHOD,
                     "the method with id: " + id +
                         " with the given parameters does not exist");
}

const std::expected<std::vector<Method>, Error>
MethodTable::getMethods(const std::string &id) const noexcept {
  if (table_.count(id)) {
    return table_.at(id);
  }
  return createError(ERROR_TYPE::METHOD,
                     "the methods with name: " + id + " do not exists");
}

std::expected<std::monostate, Error>
MethodTable::insert(const Method &method) noexcept {
  if (has(method)) {
    return createError(ERROR_TYPE::METHOD,
                       "the method: " + method.id() + " already exists");
  }
  table_[method.id()].push_back(method);
  return {};
}

void MethodTable::print() const noexcept {
  std::cout << "Methods:\n";
  for (const auto &methods : table_) {
    for (const auto &method : methods.second) {
      std::cout << method.id() << "\n";
    }
  }
}

} // namespace nicole
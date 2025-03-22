#include "../../../inc/tables/typeTable/types/userTypes/methodTable.h"

namespace nicole {

std::vector<Method>
MethodTable::getMethods(const std::string &id) const noexcept {
  if (table_.count(id)) {
    return table_.at(id);
  }
  return {};
}

void MethodTable::insert(const Method &method) noexcept {
  table_[method.id()].push_back(method);
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
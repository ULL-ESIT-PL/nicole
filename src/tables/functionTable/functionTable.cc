#include "../../../inc/tables/functionTable/functionTable.h"

namespace nicole {

std::vector<Function>
FunctionTable::getFunctions(const std::string &id) const noexcept {
  if (table_.count(id)) {
    return table_.at(id);
  }
  return {};
}

void FunctionTable::insert(const Function &function) noexcept {
  table_[function.id()].push_back(function);
}

void FunctionTable::print() const noexcept {
  std::cout << "Functions:\n";
  for (const auto &functions : table_) {
    for (const auto &function : functions.second) {
      std::cout << function.id() << "\n";
    }
  }
}

} // namespace nicole
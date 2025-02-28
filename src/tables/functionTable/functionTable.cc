#include "../../../inc/tables/functionTable/functionTable.h"

namespace nicole {

bool FunctionTable::has(const Function &function) const noexcept {
  if (!table_.count(function.id())) {
    return false;
  }
  const auto vec{table_.at(function.id())};
  for (const auto &func : vec) {
    if (func == function) {
      return true;
    }
  }
  return false;
}

const std::expected<Function, Error>
FunctionTable::getFunction(const std::string &id,
                           const Parameters &params) const noexcept {
  if (!table_.count(id)) {
    return createError(ERROR_TYPE::FUNCTION,
                       "the function with id: " + id + " does not exist");
  }
  const Function aux{id, {}, params, nullptr};
  for (const auto &func : table_.at(id)) {
    if (func == aux) { // Se asume que Parameters tiene operator==
      return func;
    }
  }
  return createError(ERROR_TYPE::FUNCTION,
                     "the function with id: " + id +
                         " with the given parameters does not exist");
}

const std::expected<std::vector<Function>, Error>
FunctionTable::getFunctions(const std::string &id) const noexcept {
  if (table_.count(id)) {
    return table_.at(id);
  }
  return createError(ERROR_TYPE::FUNCTION,
                     "the functions with name: " + id + " do not exists");
}

std::expected<std::monostate, Error>
FunctionTable::insert(const Function &function) noexcept {
  if (has(function)) {
    return createError(ERROR_TYPE::FUNCTION,
                       "the function: " + function.id() + " already exists");
  }
  table_[function.id()].push_back(function);
  return {};
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
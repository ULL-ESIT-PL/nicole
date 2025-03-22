#include "../../../inc/tables/functionTable/functionTable.h"

namespace nicole {

bool FunctionTable::has(const Function &function) const noexcept {
  if (!table_.count(function.id())) {
    return false;
  }
  const auto vec{table_.at(function.id())};
  for (const auto &func : vec) {
    if (areAmbiguous(func, function)) {
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
  const Function aux{id, {}, params, nullptr, nullptr};
  for (const auto &func : table_.at(id)) {
    if (areAmbiguous(func, aux)) {
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

bool FunctionTable::areAmbiguous(const Function &first,
                                 const Function &second) const noexcept {
  if (first.id() != second.id()) {
    return false;
  }
  if (first.generics().size() != second.generics().size()) {
    return false;
  }
  if (first.params().size() != second.params().size()) {
    return false;
  }
  const auto parameters = first.params().params();
  const auto parametersOther = second.params().params();
  for (size_t i = 0; i < parameters.size(); ++i) {
    // Si los tipos no son iguales...
    if (!typeTable_->areSameType(parameters[i].second, parametersOther[i].second)) {
      // ... pero ambos son genéricos, se omite la diferencia
      if (!(typeTable_->isGenericType(parameters[i].second, first.generics()) &&
            typeTable_->isGenericType(parametersOther[i].second, second.generics()))) {
        return false;
      }
    }
  }
  return true;
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
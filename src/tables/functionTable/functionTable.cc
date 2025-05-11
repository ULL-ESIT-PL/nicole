#include "../../../inc/tables/functionTable/functionTable.h"
#include <iostream>

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
  for (const std::pair<const std::string, std::vector<nicole::Function>>
           &functions : table_) {
    for (const Function &function : functions.second) {
      std::cout << function.id() << "\n";
    }
  }
}

std::expected<std::monostate, Error>
FunctionTable::setFuncReturnType(const std::string &id,
                                 const std::shared_ptr<Type> &type) noexcept {
  // Comprobar que existe alguna función con ese id.
  if (table_.find(id) == table_.end() || table_.at(id).empty()) {
    return createError(ERROR_TYPE::FUNCTION, "Function not found: " + id);
  }
  // Actualizamos la última función insertada con ese id.
  table_[id].back().setReturnType(type);
  return std::expected<std::monostate, Error>{std::monostate{}};
}

std::expected<std::monostate, Error>
FunctionTable::setFuncParameters(const std::string &id,
                                 const Parameters &type) noexcept {
  // Comprobar que existe alguna función con ese id.
  if (table_.find(id) == table_.end() || table_.at(id).empty()) {
    return createError(ERROR_TYPE::FUNCTION, "Function not found: " + id);
  }
  // Actualizamos los parámetros de la última función insertada con ese id.
  // Se asume que el setter de parámetros acepta un std::shared_ptr<Type>.
  table_[id].back().setParameters(type);
  return std::expected<std::monostate, Error>{std::monostate{}};
}

std::expected<std::string, Error>
FunctionTable::nameMangling(const Function &type) const noexcept {
  std::string result{"$"};
  return nameManglingImpl(type, result);
}

std::expected<std::string, Error>
FunctionTable::nameManglingImpl(const Function &type,
                                std::string &result) const noexcept {
  if (type.returnType()) {
  }
  return result;
}

} // namespace nicole
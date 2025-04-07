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

std::expected<std::monostate, Error>
MethodTable::setMethodReturnType(const std::string &id,
                                 const std::shared_ptr<Type> &type) noexcept {
  if (table_.find(id) == table_.end() || table_.at(id).empty()) {
    return createError(ERROR_TYPE::METHOD, "Method not found: " + id);
  }
  // Se actualiza la última entrada del método con el nuevo tipo de retorno.
  table_[id].back().setReturnType(type);
  return std::expected<std::monostate, Error>{std::monostate{}};
}

std::expected<std::monostate, Error>
MethodTable::setMethodParameters(const std::string &id,
                                 const Parameters &params) noexcept {
  if (table_.find(id) == table_.end() || table_.at(id).empty()) {
    return createError(ERROR_TYPE::METHOD, "Method not found: " + id);
  }
  // Se actualizan los parámetros del método.
  table_[id].back().setParameters(params);
  return std::expected<std::monostate, Error>{std::monostate{}};
}

std::expected<std::monostate, Error>
MethodTable::setMethodGenercis(const std::string &id,
                               const std::vector<GenericParameter> &params) noexcept {
  if (table_.find(id) == table_.end() || table_.at(id).empty()) {
    return createError(ERROR_TYPE::METHOD, "Method not found: " + id);
  }
  // Se actualiza la lista de genéricos del método.
  table_[id].back().setGenerics(params);
  return std::expected<std::monostate, Error>{std::monostate{}};
}


} // namespace nicole
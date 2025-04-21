#include "../../../inc/visitors/monomorphize/monomorphize.h"
#include "../../../inc/parsingAnalysis/ast/tree.h"
#include <variant>

/**

FillSemanticInfo ---> insertar delcaraciones en las tablas / insertar tipos /
comprobar que las variables pertenecen al scope (llamadas a variables) /
comrpobar llamadas a enum

TypeAnalysis ---> comprobar en una llamada a funcion que esta existe debido a
sobrecarga de funciones requiere que se trate en el typeAnalysis / igual con
metodos / llamadas a atributos / variables auto

 */

namespace nicole {

std::expected<std::string, Error>
Monomorphize::nameMangling(const std::shared_ptr<Type> &type) const noexcept {
  std::string mangled{};
  auto res = nameManglingImpl(type, mangled);
  if (!res)
    return res;
  // quitar posible guión bajo inicial
  if (!mangled.empty() && mangled.front() == '_')
    mangled.erase(mangled.begin());
  return mangled;
}

std::expected<std::string, Error>
Monomorphize::nameManglingImpl(const std::shared_ptr<Type> &type,
                               std::string &result) const noexcept {
  if (!type)
    return createError(ERROR_TYPE::TYPE, "null type in name mangling");

  if (auto bt = std::dynamic_pointer_cast<BasicType>(type)) {
    result += '_' + bt->toString();
    return result;
  }
  if (std::dynamic_pointer_cast<VoidType>(type)) {
    result += "_void";
    return result;
  }
  if (std::dynamic_pointer_cast<NullType>(type)) {
    result += "_null";
    return result;
  }
  if (auto pt = std::dynamic_pointer_cast<PointerType>(type)) {
    auto rec = nameManglingImpl(pt->baseType(), result);
    if (!rec)
      return rec;
    result += "_ptr";
    return result;
  }
  if (auto vt = std::dynamic_pointer_cast<VectorType>(type)) {
    auto rec = nameManglingImpl(vt->elementType(), result);
    if (!rec)
      return rec;
    result += "_vec";
    return result;
  }
  if (auto ct = std::dynamic_pointer_cast<ConstType>(type)) {
    result += "_const";
    auto rec = nameManglingImpl(ct->baseType(), result);
    if (!rec)
      return rec;
    return result;
  }
  if (auto et = std::dynamic_pointer_cast<EnumType>(type)) {
    result += '_' + et->name();
    return result;
  }
  if (auto git = std::dynamic_pointer_cast<GenericInstanceType>(type)) {
    result += '_' + git->name();
    for (auto &arg : git->typeArgs()) {
      auto rec = nameManglingImpl(arg, result);
      if (!rec)
        return rec;
    }
    return result;
  }
  if (auto ut = std::dynamic_pointer_cast<UserType>(type)) {
    result += '_' + ut->name();
    return result;
  }
  // Fallback
  std::string raw = type->toString();
  for (char c : raw) {
    result += (std::isalnum(static_cast<unsigned char>(c)) ? c : '_');
  }
  return result;
}

std::expected<std::string, Error>
Monomorphize::nameManglingFunction(const Function &func,
                                   const std::vector<std::shared_ptr<Type>>
                                       &genericReplacements) const noexcept {
  std::string mangled{};
  auto res = nameManglingFunctionImpl(func, genericReplacements, mangled);
  if (!res)
    return res;
  if (!mangled.empty() && mangled.front() == '_')
    mangled.erase(mangled.begin());
  return mangled;
}

std::expected<std::string, Error>
Monomorphize::nameManglingFunctionImpl(
    const Function &func,
    const std::vector<std::shared_ptr<Type>> &genericReplacements,
    std::string &result) const noexcept {
  result += '_';
  result += func.id();

  for (const auto &genType : genericReplacements) {
    auto resType = nameMangling(genType);
    if (!resType)
      return createError(resType.error());
    result += '_';
    result += *resType;
  }

  for (const auto &param : func.params().params()) {
    auto resParam = nameMangling(param.second);
    if (!resParam)
      return createError(resParam.error());
    result += '_';
    result += *resParam;
  }

  auto resRet = nameMangling(func.returnType());
  if (!resRet)
    return createError(resRet.error());
  result += "_ret_";
  result += *resRet;

  return result;
}

std::vector<GenericParameter> Monomorphize::mergeGenericLists(
    const std::vector<GenericParameter> &list,
    const std::vector<GenericParameter> &list1) const noexcept {
  std::vector<GenericParameter> result{};
  result.insert(result.end(), list.begin(), list.end());
  result.insert(result.end(), list1.begin(), list1.end());
  return result;
}

std::expected<std::monostate, Error>
Monomorphize::visit(const AST_STATEMENT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_STATEMENT");
  }
  return node->expression()->accept(*this);
}

std::expected<std::monostate, Error>
Monomorphize::visit(const AST_BODY *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_BODY");
  }
  for (const auto &statement : node->body()) {
    const auto val{statement->accept(*this)};
    if (!val) {
      return createError(val.error());
    }
  }
  return {};
}

std::expected<std::monostate, Error>
Monomorphize::visit(const Tree *tree) const noexcept {
  if (!tree) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid Tree");
  }
  return tree->root()->accept(*this);
}

} // namespace nicole
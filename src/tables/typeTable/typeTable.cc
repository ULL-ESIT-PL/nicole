#include "../../../inc/tables/typeTable/typeTable.h"
#include <memory>

namespace nicole {

bool TypeTable::has(const std::string &id) const noexcept {
  return table_.count(id);
}

const std::expected<std::shared_ptr<Type>, Error>
TypeTable::getType(const std::string &id) const noexcept {
  if (has(id)) {
    return table_.at(id);
  }
  return createError(ERROR_TYPE::TYPE, "the type: " + id + " does not exists");
}

std::expected<std::monostate, Error>
TypeTable::insert(const std::shared_ptr<Type> &type) noexcept {
  if (has(type->toString())) {
    return createError(ERROR_TYPE::TYPE,
                       "the type: " + type->toString() + " already exists");
  }
  table_[type->toString()] = type;
  return {};
}

bool TypeTable::isPossibleType(
    const std::shared_ptr<Type> &type) const noexcept {
  if (const auto &basicType = std::dynamic_pointer_cast<BasicType>(type)) {
    return true;
  } else if (const auto &voidType = std::dynamic_pointer_cast<VoidType>(type)) {
    return true;
  } else if (const auto &vectorType =
                 std::dynamic_pointer_cast<VectorType>(type)) {
    return isPossibleType(vectorType->elementType());
  } else if (const auto &pointerType =
                 std::dynamic_pointer_cast<PointerType>(type)) {
    return isPossibleType(pointerType->baseType());
  } else if (const auto &constType =
                 std::dynamic_pointer_cast<ConstType>(type)) {
    return isPossibleType(constType->baseType());
  } else if (const auto &nullType = std::dynamic_pointer_cast<NullType>(type)) {
    return true;
  } else if (const auto &userType = std::dynamic_pointer_cast<UserType>(type)) {
    if (!has(userType->name())) {
      return false;
    }
    return true;
  } else if (const auto &genericInstanceType =
                 std::dynamic_pointer_cast<GenericInstanceType>(type)) {
    if (!has(genericInstanceType->genericType()->name())) {
      return false;
    }
    for (const auto &arg : genericInstanceType->typeArgs()) {
      if (!isPossibleType(arg)) {
        return false;
      }
    }
    return true;
  }
  return false;
}

bool TypeTable::isGenericType(
    const std::shared_ptr<Type> &type,
    const std::vector<GenericParameter> &generics) const noexcept {
  if (const auto &vectorType =
                 std::dynamic_pointer_cast<VectorType>(type)) {
    return isGenericType(vectorType->elementType(), generics);
  } else if (const auto &pointerType =
                 std::dynamic_pointer_cast<PointerType>(type)) {
    return isGenericType(pointerType->baseType(), generics);
  } else if (const auto &constType =
                 std::dynamic_pointer_cast<ConstType>(type)) {
    return isGenericType(constType->baseType(), generics);
  } else if (const auto &userType = std::dynamic_pointer_cast<UserType>(type)) {
    for (const auto& generic : generics) {
      if (generic.name() == userType->name()) {
        return true;
      }
    }
    return false;
  } else if (const auto &genericInstanceType =
                 std::dynamic_pointer_cast<GenericInstanceType>(type)) {
    if (!has(genericInstanceType->genericType()->name())) {
      return false;
    }
    for (const auto &arg : genericInstanceType->typeArgs()) {
      if (!isGenericType(arg, generics)) {
        return false;
      }
    }
    return true;
  }
  return false;
}

} // namespace nicole
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
  if (const auto &enumType = std::dynamic_pointer_cast<EnumType>(type)) {
    table_[enumType->name()] = type;
    return {};
  }
  const auto &userType = std::dynamic_pointer_cast<UserType>(type);
  table_[userType->name()] = type;
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
  } else if (const auto &enumType = std::dynamic_pointer_cast<EnumType>(type)) {
    if (!has(enumType->name())) {
      return false;
    }
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
  if (const auto &vectorType = std::dynamic_pointer_cast<VectorType>(type)) {
    return isGenericType(vectorType->elementType(), generics);
  } else if (const auto &pointerType =
                 std::dynamic_pointer_cast<PointerType>(type)) {
    return isGenericType(pointerType->baseType(), generics);
  } else if (const auto &constType =
                 std::dynamic_pointer_cast<ConstType>(type)) {
    return isGenericType(constType->baseType(), generics);
  } else if (const auto &enumType = std::dynamic_pointer_cast<EnumType>(type)) {
    for (const auto &generic : generics) {
      if (generic.name() == enumType->name()) {
        return true;
      }
    }
    return false;
  } else if (const auto &userType = std::dynamic_pointer_cast<UserType>(type)) {
    for (const auto &generic : generics) {
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

bool TypeTable::areSameType(const std::shared_ptr<Type> &type1,
                            const std::shared_ptr<Type> &type2) const noexcept {
  // Si ambos apuntan al mismo objeto, son iguales.
  if (type1 == type2) {
    return true;
  }
  // Si alguno es nulo, no son iguales.
  if (!type1 || !type2) {
    return false;
  }

  // Caso BasicType
  if (auto basic1 = std::dynamic_pointer_cast<BasicType>(type1)) {
    if (auto basic2 = std::dynamic_pointer_cast<BasicType>(type2)) {
      return basic1->baseKind() == basic2->baseKind();
    }
    return false;
  }

  // Caso VoidType
  if (std::dynamic_pointer_cast<VoidType>(type1)) {
    return std::dynamic_pointer_cast<VoidType>(type2) != nullptr;
  }

  // Caso NullType
  if (std::dynamic_pointer_cast<NullType>(type1)) {
    return std::dynamic_pointer_cast<NullType>(type2) != nullptr;
  }

  // Caso VectorType
  if (auto vec1 = std::dynamic_pointer_cast<VectorType>(type1)) {
    if (auto vec2 = std::dynamic_pointer_cast<VectorType>(type2)) {
      return areSameType(vec1->elementType(), vec2->elementType());
    }
    return false;
  }

  // Caso PointerType
  if (auto ptr1 = std::dynamic_pointer_cast<PointerType>(type1)) {
    if (auto ptr2 = std::dynamic_pointer_cast<PointerType>(type2)) {
      return areSameType(ptr1->baseType(), ptr2->baseType());
    }
    return false;
  }

  // Caso ConstType
  if (auto const1 = std::dynamic_pointer_cast<ConstType>(type1)) {
    if (auto const2 = std::dynamic_pointer_cast<ConstType>(type2)) {
      return areSameType(const1->baseType(), const2->baseType());
    }
    return false;
  }

  // Caso EnumType
  if (auto enum1 = std::dynamic_pointer_cast<EnumType>(type1)) {
    if (auto enum2 = std::dynamic_pointer_cast<EnumType>(type2)) {
      return enum1->name() == enum2->name();
    }
    return false;
  }

  // Caso UserType
  if (auto user1 = std::dynamic_pointer_cast<UserType>(type1)) {
    if (auto user2 = std::dynamic_pointer_cast<UserType>(type2)) {
      return user1->name() == user2->name();
    }
    return false;
  }

  // Caso GenericInstanceType
  if (auto genInst1 = std::dynamic_pointer_cast<GenericInstanceType>(type1)) {
    if (auto genInst2 = std::dynamic_pointer_cast<GenericInstanceType>(type2)) {
      // Se comparan primero los tipos genéricos base
      if (!areSameType(genInst1->genericType(), genInst2->genericType())) {
        return false;
      }
      const auto &args1 = genInst1->typeArgs();
      const auto &args2 = genInst2->typeArgs();
      if (args1.size() != args2.size()) {
        return false;
      }
      for (size_t i = 0; i < args1.size(); ++i) {
        if (!areSameType(args1[i], args2[i])) {
          return false;
        }
      }
      return true;
    }
    return false;
  }

  // Si no se reconoce el tipo, se compara mediante su representación canónica.
  return type1->toString() == type2->toString();
}

} // namespace nicole
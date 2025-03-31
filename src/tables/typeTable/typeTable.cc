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
  } else if (const auto &genericInstanceType =
                 std::dynamic_pointer_cast<GenericInstanceType>(type)) {
    if (!has(genericInstanceType->name())) {
      return false;
    }
    for (const auto &arg : genericInstanceType->typeArgs()) {
      if (!isPossibleType(arg)) {
        return false;
      }
    }
    return true;
  } else if (const auto &userType = std::dynamic_pointer_cast<UserType>(type)) {
    if (!has(userType->name())) {
      return false;
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
  } else if (const auto &genericInstanceType =
                 std::dynamic_pointer_cast<GenericInstanceType>(type)) {
    if (!has(genericInstanceType->name())) {
      return false;
    }
    for (const auto &arg : genericInstanceType->typeArgs()) {
      if (!isGenericType(arg, generics)) {
        return false;
      }
    }
    return true;
  } else if (const auto &userType = std::dynamic_pointer_cast<UserType>(type)) {
    for (const auto &generic : generics) {
      if (generic.name() == userType->name()) {
        return true;
      }
    }
    return false;
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

  // Caso GenericInstanceType
  if (auto genInst1 = std::dynamic_pointer_cast<GenericInstanceType>(type1)) {
    if (auto genInst2 = std::dynamic_pointer_cast<GenericInstanceType>(type2)) {
      // Se comparan primero los tipos genéricos base
      if (!areSameType(genInst1, genInst2)) {
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

  // Caso UserType
  if (auto user1 = std::dynamic_pointer_cast<UserType>(type1)) {
    if (auto user2 = std::dynamic_pointer_cast<UserType>(type2)) {
      return user1->name() == user2->name();
    }
    return false;
  }

  // Si no se reconoce el tipo, se compara mediante su representación canónica.
  return type1->toString() == type2->toString();
}

bool TypeTable::canAssign(const std::shared_ptr<Type> &target,
                          const std::shared_ptr<Type> &source) const noexcept {
  // Si son exactamente iguales, se puede asignar.
  if (areSameType(target, source))
    return true;

  // Permitir asignar null a cualquier puntero.
  if (std::dynamic_pointer_cast<NullType>(source)) {
    if (std::dynamic_pointer_cast<PointerType>(target))
      return true;
  }

  // Desempaquetar ConstType para comparar los tipos subyacentes.
  std::shared_ptr<Type> tTarget = target;
  std::shared_ptr<Type> tSource = source;
  if (auto constTarget = std::dynamic_pointer_cast<ConstType>(target))
    tTarget = constTarget->baseType();
  if (auto constSource = std::dynamic_pointer_cast<ConstType>(source))
    tSource = constSource->baseType();

  // Si ambos son punteros, se verifica la asignabilidad de sus tipos base.
  if (auto targetPtr = std::dynamic_pointer_cast<PointerType>(tTarget)) {
    if (auto sourcePtr = std::dynamic_pointer_cast<PointerType>(tSource))
      return canAssign(targetPtr->baseType(), sourcePtr->baseType());
  }

  // Si ambos son GenericInstanceType, se compara tanto el tipo base (UserType)
  // como la lista de argumentos, de forma recursiva.
  auto targetGenInst = std::dynamic_pointer_cast<GenericInstanceType>(tTarget);
  auto sourceGenInst = std::dynamic_pointer_cast<GenericInstanceType>(tSource);
  if (targetGenInst && sourceGenInst) {
    if (targetGenInst->name() != sourceGenInst->name())
      return false;
    const auto &argsTarget = targetGenInst->typeArgs();
    const auto &argsSource = sourceGenInst->typeArgs();
    if (argsTarget.size() != argsSource.size())
      return false;
    for (size_t i = 0; i < argsTarget.size(); ++i) {
      if (!canAssign(argsTarget[i], argsSource[i]))
        return false;
    }
    return true;
  }

  // Si uno de ellos es GenericInstanceType pero ambos son tipos de usuario,
  // se permite la asignación si el tipo fuente es el mismo o hereda
  // (polimorfismo) del destino. Se recorre la cadena de herencia (baseType).
  if (auto targetUser = std::dynamic_pointer_cast<UserType>(tTarget)) {
    if (auto sourceUser = std::dynamic_pointer_cast<UserType>(tSource)) {
      for (auto current = sourceUser; current; current = current->baseType()) {
        if (current->name() == targetUser->name())
          return true;
      }
    }
  }

  // Para BasicType, se verifica que ambos tengan el mismo "baseKind".
  if (auto targetBasic = std::dynamic_pointer_cast<BasicType>(tTarget)) {
    if (auto sourceBasic = std::dynamic_pointer_cast<BasicType>(tSource))
      return targetBasic->baseKind() == sourceBasic->baseKind();
  }

  // Para EnumType, se permite la asignación si tienen el mismo nombre.
  if (auto targetEnum = std::dynamic_pointer_cast<EnumType>(tTarget)) {
    if (auto sourceEnum = std::dynamic_pointer_cast<EnumType>(tSource))
      return targetEnum->name() == sourceEnum->name();
  }

  // Para VectorType, se verifica que sus tipos de elementos sean asignables.
  if (auto targetVec = std::dynamic_pointer_cast<VectorType>(tTarget)) {
    if (auto sourceVec = std::dynamic_pointer_cast<VectorType>(tSource))
      return canAssign(targetVec->elementType(), sourceVec->elementType());
  }

  return false;
}

bool TypeTable::haveCommonAncestor(
    const std::shared_ptr<Type> &type1,
    const std::shared_ptr<Type> &type2) const noexcept {
  // Solo tiene sentido para tipos de usuario (incluyendo GenericInstanceType,
  // que se tratan como UserType)
  auto user1 = std::dynamic_pointer_cast<UserType>(type1);
  auto user2 = std::dynamic_pointer_cast<UserType>(type2);
  if (user1 && user2) {
    // Comprobar si user1 es ancestro de user2
    for (auto current = user2; current; current = current->baseType()) {
      if (current->name() == user1->name()) {
        return true;
      }
    }
    // O si user2 es ancestro de user1
    for (auto current = user1; current; current = current->baseType()) {
      if (current->name() == user2->name()) {
        return true;
      }
    }
  }
  return false;
}

} // namespace nicole
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

std::expected<std::shared_ptr<Type>, Error>
TypeTable::isCompundEnumType(const std::shared_ptr<Type> &type) const noexcept {
  if (auto constType = std::dynamic_pointer_cast<ConstType>(type)) {
    auto baseRes = isCompundEnumType(constType->baseType());
    if (!baseRes)
      return createError(baseRes.error());
    return std::make_shared<ConstType>(baseRes.value());
  }
  if (auto pointerType = std::dynamic_pointer_cast<PointerType>(type)) {
    auto baseRes = isCompundEnumType(pointerType->baseType());
    if (!baseRes)
      return createError(baseRes.error());
    return std::make_shared<PointerType>(baseRes.value());
  }
  if (auto userType = std::dynamic_pointer_cast<UserType>(type)) {
    auto exists = getType(userType->name());
    if (!exists)
      return createError(exists.error());
    if (auto enumType = std::dynamic_pointer_cast<EnumType>(exists.value()))
      return enumType;
    else
      return createError(ERROR_TYPE::TYPE,
                         "El tipo encontrado no es un EnumType");
  }
  return createError(ERROR_TYPE::TYPE, "El tipo no es un Enum compuesto");
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

// Función pública que no requiere el tercer parámetro.
bool TypeTable::canAssign(const std::shared_ptr<Type> &target,
                          const std::shared_ptr<Type> &source) const noexcept {
  return canAssignImpl(target, source, false);
}

// Función interna que maneja la lógica completa, incluyendo el parámetro
// pointerContext.
bool TypeTable::canAssignImpl(const std::shared_ptr<Type> &target,
                              const std::shared_ptr<Type> &source,
                              bool pointerContext) const noexcept {
  if (areSameType(target, source))
    return true;

  if (std::dynamic_pointer_cast<NullType>(source)) {
    if (std::dynamic_pointer_cast<PointerType>(target))
      return true;
  }

  std::shared_ptr<Type> tTarget = target;
  std::shared_ptr<Type> tSource = source;
  if (auto constTarget = std::dynamic_pointer_cast<ConstType>(target))
    tTarget = constTarget->baseType();
  if (auto constSource = std::dynamic_pointer_cast<ConstType>(source))
    tSource = constSource->baseType();

  // Manejo de PlaceHolder (omitido para brevedad).

  if (auto targetPtr = std::dynamic_pointer_cast<PointerType>(tTarget)) {
    if (auto sourcePtr = std::dynamic_pointer_cast<PointerType>(tSource))
      return canAssignImpl(targetPtr->baseType(), sourcePtr->baseType(), true);
  }

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
      if (!canAssignImpl(argsTarget[i], argsSource[i], pointerContext))
        return false;
    }
    return true;
  }

  if (auto targetUser = std::dynamic_pointer_cast<UserType>(tTarget)) {
    if (auto sourceUser = std::dynamic_pointer_cast<UserType>(tSource)) {
      // Se permite A = A sin importar el contexto.
      if (targetUser->name() == sourceUser->name())
        return true;
      // Fuera del contexto de punteros no se admite la asignación basada en
      // herencia.
      if (!pointerContext)
        return false;
      // En contexto de punteros se permite que B herede de A.
      for (auto current = sourceUser; current; current = current->baseType()) {
        if (current->name() == targetUser->name())
          return true;
      }
    }
  }

  if (auto targetBasic = std::dynamic_pointer_cast<BasicType>(tTarget)) {
    if (auto sourceBasic = std::dynamic_pointer_cast<BasicType>(tSource))
      return targetBasic->baseKind() == sourceBasic->baseKind();
  }

  if (auto targetEnum = std::dynamic_pointer_cast<EnumType>(tTarget)) {
    if (auto sourceEnum = std::dynamic_pointer_cast<EnumType>(tSource))
      return targetEnum->name() == sourceEnum->name();
  }

  if (auto targetVec = std::dynamic_pointer_cast<VectorType>(tTarget)) {
    if (auto sourceVec = std::dynamic_pointer_cast<VectorType>(tSource))
      return canAssignImpl(targetVec->elementType(), sourceVec->elementType(),
                           pointerContext);
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

// these two methods are still unfinished
std::expected<std::shared_ptr<Type>, Error>
TypeTable::applyUnaryOperator(const std::shared_ptr<Type> &operand,
                              const TokenType op) const noexcept {
  if (!operand)
    return createError(ERROR_TYPE::TYPE, "operand is null");

  std::shared_ptr<Type> t = operand;
  if (auto constType = std::dynamic_pointer_cast<ConstType>(t))
    t = constType->baseType();

  if (std::dynamic_pointer_cast<PlaceHolder>(t))
    return createError(
        ERROR_TYPE::TYPE,
        "cannot apply unary operator to unresolved generic type");

  // Si el operando es un tipo definido por el usuario, se busca el método
  // correspondiente sin parámetros.
  if (auto userType = std::dynamic_pointer_cast<UserType>(t)) {
    std::string methodName;
    switch (op) {
    case TokenType::OPERATOR_NOT:
      methodName = "notable";
      break;
    case TokenType::INCREMENT:
      methodName = "incrementable";
      break;
    case TokenType::DECREMENT:
      methodName = "decrementable";
      break;
    case TokenType::OPERATOR_SUB:
      methodName = "negable";
      break;
    default:
      return createError(ERROR_TYPE::TYPE,
                         "unsupported unary operator for user-defined type");
    }
    auto methodsExp = userType->getMethods(methodName);
    if (!methodsExp)
      return createError(methodsExp.error());
    auto methods = methodsExp.value();
    for (const auto &m : methods) {
      if (m.params().size() == 0) {
        // Para operator_not, el resultado debe ser bool.
        if (op == TokenType::OPERATOR_NOT) {
          auto boolTypeExp = getType("bool");
          if (!boolTypeExp)
            return createError(ERROR_TYPE::TYPE, "bool type not found");
          if (!areSameType(m.returnType(), *boolTypeExp))
            return createError(ERROR_TYPE::TYPE, "operator ! must return bool");
          return boolTypeExp.value();
        }
        // Para los otros operadores, se retorna el tipo que devuelve el método.
        return m.returnType();
      }
    }
    return createError(ERROR_TYPE::TYPE, "user type " + userType->name() +
                                             " does not support operator " +
                                             tokenTypeToString(op));
  }

  // Si es un PointerType.
  if (auto ptrType = std::dynamic_pointer_cast<PointerType>(t)) {
    switch (op) {
    case TokenType::INCREMENT:
    case TokenType::DECREMENT:
      return t; // Se retorna el mismo tipo puntero.
    case TokenType::OPERATOR_NOT: {
      auto boolTypeExp = getType("bool");
      if (!boolTypeExp)
        return createError(ERROR_TYPE::TYPE, "bool type not found");
      return boolTypeExp.value();
    }
    default:
      return createError(ERROR_TYPE::TYPE,
                         "unsupported unary operator for pointer type");
    }
  }

  // Si es un BasicType.
  if (auto basic = std::dynamic_pointer_cast<BasicType>(t)) {
    switch (op) {
    case TokenType::OPERATOR_NOT: {
      // El resultado de ! siempre es bool.
      auto boolTypeExp = getType("bool");
      if (!boolTypeExp)
        return createError(ERROR_TYPE::TYPE, "bool type not found");
      return boolTypeExp.value();
    }
    case TokenType::INCREMENT:
    case TokenType::DECREMENT:
    case TokenType::OPERATOR_SUB:
      // Verificar que sea numérico (o char, asumido como numérico).
      switch (basic->baseKind()) {
      case BasicKind::Int:
      case BasicKind::Float:
      case BasicKind::Double:
      case BasicKind::Char:
        return t;
      default:
        return createError(ERROR_TYPE::TYPE,
                           "operator " + tokenTypeToString(op) +
                               " cannot be applied to type " + t->toString());
      }
    default:
      return createError(ERROR_TYPE::TYPE, "unsupported unary operator");
    }
  }

  return createError(ERROR_TYPE::TYPE,
                     "unary operator cannot be applied to type " +
                         t->toString());
}

std::expected<std::shared_ptr<Type>, Error>
TypeTable::applyBinaryOperator(const std::shared_ptr<Type> &left,
                               const std::shared_ptr<Type> &right,
                               TokenType op) const noexcept {
  std::shared_ptr<Type> l = left;
  std::shared_ptr<Type> r = right;

  if (auto lConst = std::dynamic_pointer_cast<ConstType>(l))
    l = lConst->baseType();
  if (auto rConst = std::dynamic_pointer_cast<ConstType>(r))
    r = rConst->baseType();

  switch (op) {
  case TokenType::OPERATOR_ADD: {
    auto leftBasic = std::dynamic_pointer_cast<BasicType>(l);
    auto rightBasic = std::dynamic_pointer_cast<BasicType>(r);
    if (leftBasic && rightBasic) {
      if (areSameType(l, r))
        return l;
      else
        return createError(ERROR_TYPE::TYPE,
                           "incompatible types for operator +");
    }
    // Extend here for string concatenation, pointer arithmetic, etc.
    return createError(ERROR_TYPE::TYPE,
                       "operator + not applicable for given types");
  }
  // Cases for other operators (OR, AND, SMALLEREQUAL, BIGGEREQUAL, NOTEQUAL,
  // EQUAL, OPERATOR_SUB, OPERATOR_MULT, OPERATOR_DIV, OPERATOR_SMALLER,
  // OPERATOR_GREATER, OPERATOR_MODULE)
  default:
    return createError(ERROR_TYPE::TYPE,
                       "operator not implemented: " + tokenTypeToString(op));
  }
}

} // namespace nicole
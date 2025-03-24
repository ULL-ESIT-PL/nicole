#ifndef PTR_VALIDATOR_H
#define PTR_VALIDATOR_H

#include "baseTypeValidator.h"
#include <memory>

namespace nicole {

class PtrValidator final : public BaseTypeValidator {
public:
  ~PtrValidator() override = default;

  // Solo es asignable si el puntero fuente es del mismo tipo (o en la misma jerarquía)
  [[nodiscard]] std::shared_ptr<Type>
  isAssignable(const std::shared_ptr<Type>& source) const noexcept override {
    // Obtenemos el PointerType asociado a este validador.
    auto targetPtr = std::dynamic_pointer_cast<PointerType>(thisType());
    if (!targetPtr)
      return nullptr;

    // El source debe ser también un PointerType.
    auto sourcePtr = std::dynamic_pointer_cast<PointerType>(source);
    if (!sourcePtr)
      return nullptr;

    // Verificamos que los tipos base sean iguales o compatibles (dentro de la misma jerarquía).
    if (isSameOrDerived(targetPtr->baseType(), sourcePtr->baseType())) {
      return targetPtr; // Se permite la asignación.
    }
    return nullptr;
  }

  // Es convertible si el tipo apuntado es un UserType o una GenericInstanceType.
  [[nodiscard]] std::shared_ptr<Type>
  isConvertible(const std::shared_ptr<Type>&) const noexcept override {
    auto targetPtr = std::dynamic_pointer_cast<PointerType>(thisType());
    if (!targetPtr)
      return nullptr;

    auto base = targetPtr->baseType();
    if (std::dynamic_pointer_cast<UserType>(base) ||
        std::dynamic_pointer_cast<GenericInstanceType>(base)) {
      return targetPtr; // Se permite la conversión.
    }
    return nullptr;
  }

  // Es operable (en comparación) si se usa EQUAL o NOTEQUAL y se compara con null o con otro puntero del mismo tipo base.
  [[nodiscard]] std::shared_ptr<Type>
  isOperable(const std::shared_ptr<Type>& other, const Token &tk) const noexcept override {
    // Solo se permiten operaciones de comparación: == o !=.
    if (tk.type() != TokenType::EQUAL && tk.type() != TokenType::NOTEQUAL)
      return nullptr;

    auto targetPtr = std::dynamic_pointer_cast<PointerType>(thisType());
    if (!targetPtr)
      return nullptr;

    // Si el otro operando es null (suponiendo que se representa como nullptr en el sistema de tipos),
    // se permite la comparación y se retorna el tipo booleano.
    if (!other)
      return std::make_shared<BasicType>(BasicKind::Bool);

    auto otherPtr = std::dynamic_pointer_cast<PointerType>(other);
    if (!otherPtr)
      return nullptr;

    // Permitir la comparación si ambos punteros apuntan a tipos compatibles (considerando const si es aplicable).
    if (areSameType(targetPtr->baseType(), otherPtr->baseType())) {
      return std::make_shared<BasicType>(BasicKind::Bool);
    }
    return nullptr;
  }

private:
  // Se asume que cada PtrValidator está asociado a un PointerType concreto.
  // Este método debe devolver el PointerType para el cual se está validando.
  [[nodiscard]] std::shared_ptr<Type> thisType() const noexcept {
    // La implementación depende de cómo se asocia el validador a su tipo.
    // Por ejemplo, se podría tener un campo miembro:
    // return associatedPointerType_;
    // En este ejemplo se retorna nullptr para ilustrar la idea.
    return nullptr;
  }

  // Verifica si target y source son iguales o si source es derivado de target.
  bool isSameOrDerived(const std::shared_ptr<Type>& target,
                       const std::shared_ptr<Type>& source) const noexcept {
    return areSameType(target, source) || isDerivedFrom(source, target);
  }

  // Verifica igualdad de tipos de forma simplificada (idealmente usando la forma canónica).
  bool areSameType(const std::shared_ptr<Type>& t1,
                   const std::shared_ptr<Type>& t2) const noexcept {
    return t1 && t2 && (t1->toString() == t2->toString());
  }

  // Verifica si 'source' es derivado de 'target' (para UserType, se puede usar isAboveInHearchy).
  bool isDerivedFrom(const std::shared_ptr<Type>& source,
                     const std::shared_ptr<Type>& target) const noexcept {
    auto userSource = std::dynamic_pointer_cast<UserType>(source);
    auto userTarget = std::dynamic_pointer_cast<UserType>(target);
    if (userSource && userTarget) {
      return userSource->isAboveInHearchy(userTarget);
    }
    return false;
  }
};

} // namespace nicole

#endif

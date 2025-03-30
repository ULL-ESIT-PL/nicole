#ifndef TYPE_MANAGER_H
#define TYPE_MANAGER_H

#include "typeTable.h"
#include "typeValidators/boolValidator.h"
#include "typeValidators/boolValidator.h"
#include "typeValidators/charValidator.h"
#include "typeValidators/constValidator.h"
#include "typeValidators/doubleValidator.h"
#include "typeValidators/enumValidator.h"
#include "typeValidators/floatValidator.h"
#include "typeValidators/genericInstanceValidator.h"
#include "typeValidators/intValidator.h"
#include "typeValidators/nullValidator.h"
#include "typeValidators/ptrValidator.h"
#include "typeValidators/strValidator.h"
#include "typeValidators/userTypeValidator.h"
#include "typeValidators/vectorValidator.h"
#include "typeValidators/voidValidator.h"
#include <memory>

namespace nicole {

class TypeManager final {
private:
  std::shared_ptr<TypeTable> typeTable_;
public:
  explicit TypeManager(const std::shared_ptr<TypeTable>& typeTable) noexcept : typeTable_{typeTable} {}

  [[nodiscard]] const std::shared_ptr<TypeTable> &
  getTypeTable() const noexcept {
    return typeTable_;
  }

  [[nodiscard]] bool
  areSameType(const std::shared_ptr<Type> &type1,
              const std::shared_ptr<Type> &type2) const noexcept;

  [[nodiscard]] bool
  isPossibleConversion(const std::shared_ptr<Type> &type1,
                       const std::shared_ptr<Type> &type2) const noexcept;

  [[nodiscard]] bool
  isPossibleHierarchy(const std::shared_ptr<Type> &type1,
                      const std::shared_ptr<Type> &type2) const noexcept;

  [[nodiscard]] bool isPossibleOperation(const std::shared_ptr<Type> &type1,
                                         const std::shared_ptr<Type> &type2,
                                         const Token &op) const noexcept;

  [[nodiscard]] bool
  isAssignable(const std::shared_ptr<Type> &src,
               const std::shared_ptr<Type> &dest) const noexcept;

  // Comprueba si dos tipos pueden compararse (por igualdad o relaciones).
  [[nodiscard]] bool
  isComparable(const std::shared_ptr<Type> &type1,
               const std::shared_ptr<Type> &type2) const noexcept;

  // Determina el tipo común resultante de dos tipos, en operaciones mixtas.
  [[nodiscard]] std::expected<std::shared_ptr<Type>, Error>
  getCommonType(const std::shared_ptr<Type> &type1,
                const std::shared_ptr<Type> &type2) const noexcept;
};

} // namespace nicole

#endif

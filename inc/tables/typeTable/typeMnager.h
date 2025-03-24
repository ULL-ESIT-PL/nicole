#ifndef TYPE_MANAGER_H
#define TYPE_MANAGER_H

#include "typeTable.h"
#include "typeValidators/baseTypeValidator.h"
#include <memory>

namespace nicole {

class TypeManager final {
private:
  std::shared_ptr<TypeTable> typeTable_{std::make_shared<TypeTable>()};

  TypeManager() = default;

public:
  TypeManager(const TypeManager &) = delete;
  TypeManager &operator=(const TypeManager &) = delete;

  static TypeManager &getInstance() {
    static TypeManager instance;
    return instance;
  }

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

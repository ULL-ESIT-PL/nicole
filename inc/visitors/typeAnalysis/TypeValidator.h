#ifndef TYPE_VALIDATOR_H
#define TYPE_VALIDATOR_H

#include "../../tables/typeTable/typeTable.h"
#include "../../lexicalAnalysis/token.h"
#include <memory>

namespace nicole {

class TypeValidator final {

private:
  TypeValidator() = delete;

public:
  [[nodiscard]] static bool
  areSameType(const std::shared_ptr<Type> &type1,
              const std::shared_ptr<Type> &type2) noexcept;

  [[nodiscard]] static bool
  isPossibleConversion(const std::shared_ptr<Type> &type1,
                       const std::shared_ptr<Type> &type2) noexcept;

  [[nodiscard]] static bool
  isPossibleHierarchy(const std::shared_ptr<Type> &type1,
                      const std::shared_ptr<Type> &type2) noexcept;

  [[nodiscard]] static bool
  isPossibleOperation(const std::shared_ptr<Type> &type1,
                      const std::shared_ptr<Type> &type2,
                      const Token &op) noexcept;

  [[nodiscard]] static bool
  isAssignable(const std::shared_ptr<Type> &src,
               const std::shared_ptr<Type> &dest) noexcept;

  // Comprueba si dos tipos pueden compararse (por igualdad o relaciones).
  [[nodiscard]] static bool
  isComparable(const std::shared_ptr<Type> &type1,
               const std::shared_ptr<Type> &type2) noexcept;

  // Determina el tipo común resultante de dos tipos, en operaciones mixtas.
  [[nodiscard]] static std::expected<std::shared_ptr<Type>, Error>
  getCommonType(const std::shared_ptr<Type> &type1,
                const std::shared_ptr<Type> &type2) noexcept;
};

} // namespace nicole

#endif
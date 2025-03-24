#ifndef BASE_TYPE_VALIDATOR_H
#define BASE_TYPE_VALIDATOR_H

#include "../../../lexicalAnalysis/token.h"
#include "../typeTable.h"
#include <memory>

namespace nicole {

class BaseTypeValidator {
public:
  virtual ~BaseTypeValidator() = default;

  [[nodiscard]] virtual std::shared_ptr<Type>
  isAssignable(const std::shared_ptr<Type> &type) const noexcept = 0;

  [[nodiscard]] virtual std::shared_ptr<Type>
  isConvertible(const std::shared_ptr<Type> &type) const noexcept = 0;

  [[nodiscard]] virtual std::shared_ptr<Type> isOperable(const std::shared_ptr<Type> &type,
                                        const Token &tk) const noexcept = 0;
};

} // namespace nicole

#endif
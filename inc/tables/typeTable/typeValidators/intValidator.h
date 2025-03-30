#ifndef INT_VALIDATOR_H
#define INT_VALIDATOR_H

#include "baseTypeValidator.h"

namespace nicole {

class IntValidator final : public BaseTypeValidator {
public:
  [[nodiscard]] std::shared_ptr<Type>
  isAssignable(const std::shared_ptr<Type> &) const noexcept override;

  [[nodiscard]] std::shared_ptr<Type>
  isConvertible(const std::shared_ptr<Type> &) const noexcept override;

  [[nodiscard]] std::shared_ptr<Type>
  isOperable(const std::shared_ptr<Type> &,
             const Token &) const noexcept override;
};

} // namespace nicole

#endif
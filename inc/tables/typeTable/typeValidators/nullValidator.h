#ifndef NULL_VALIDATOR_H
#define NULL_VALIDATOR_H

#include "baseTypeValidator.h"
#include <memory>

namespace nicole {

class NullValidator final : public BaseTypeValidator {
public:
  [[nodiscard]] std::shared_ptr<Type>
  isAssignable(const std::shared_ptr<Type> &) const noexcept override;

  [[nodiscard]] std::shared_ptr<Type>
  isConvertible(const std::shared_ptr<Type> &) const noexcept override;

  [[nodiscard]] std::shared_ptr<Type>
  isOperable(const std::shared_ptr<Type> &type,
             const Token &tk) const noexcept override;
};

} // namespace nicole

#endif
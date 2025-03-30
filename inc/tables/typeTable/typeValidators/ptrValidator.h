#ifndef PTR_VALIDATOR_H
#define PTR_VALIDATOR_H

#include "baseTypeValidator.h"
#include <memory>

namespace nicole {

class PtrValidator final : public BaseTypeValidator {
public:
  ~PtrValidator() override = default;

  [[nodiscard]] std::shared_ptr<Type>
  isAssignable(const std::shared_ptr<Type> &source) const noexcept override;

  [[nodiscard]] std::shared_ptr<Type>
  isConvertible(const std::shared_ptr<Type> &) const noexcept override;

  [[nodiscard]] std::shared_ptr<Type>
  isOperable(const std::shared_ptr<Type> &other,
             const Token &tk) const noexcept override;
};

} // namespace nicole

#endif

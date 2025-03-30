#include "../../../../inc/tables/typeTable/typeValidators/userTypeValidator.h"

namespace nicole {

std::shared_ptr<Type>
UserTypeValidator::isAssignable(const std::shared_ptr<Type> &) const noexcept {
  return nullptr;
}

std::shared_ptr<Type>
UserTypeValidator::isConvertible(const std::shared_ptr<Type> &) const noexcept {
  return nullptr;
}

std::shared_ptr<Type> UserTypeValidator::isOperable(const std::shared_ptr<Type> &,
                                                const Token &) const noexcept {
  return nullptr;
};

} // namespace nicole
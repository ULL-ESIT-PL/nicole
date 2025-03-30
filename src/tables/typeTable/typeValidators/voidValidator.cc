#include "../../../../inc/tables/typeTable/typeValidators/voidValidator.h"

namespace nicole {

std::shared_ptr<Type>
VoidValidator::isAssignable(const std::shared_ptr<Type> &) const noexcept {
  return nullptr;
}

std::shared_ptr<Type>
VoidValidator::isConvertible(const std::shared_ptr<Type> &) const noexcept {
  return nullptr;
}

std::shared_ptr<Type> VoidValidator::isOperable(const std::shared_ptr<Type> &,
                                                const Token &) const noexcept {
  return nullptr;
};

} // namespace nicole
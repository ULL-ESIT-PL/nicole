#include "../../../../inc/tables/typeTable/typeValidators/ptrValidator.h"

namespace nicole {

std::shared_ptr<Type>
PtrValidator::isAssignable(const std::shared_ptr<Type> &) const noexcept {
  return nullptr;
}

std::shared_ptr<Type>
PtrValidator::isConvertible(const std::shared_ptr<Type> &) const noexcept {
  return nullptr;
}

std::shared_ptr<Type> PtrValidator::isOperable(const std::shared_ptr<Type> &,
                                                const Token &) const noexcept {
  return nullptr;
};

} // namespace nicole
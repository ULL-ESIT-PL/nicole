#include "../../../../inc/tables/typeTable/typeValidators/genericInstanceValidator.h"

namespace nicole {

std::shared_ptr<Type>
GenericInstanceValidator::isAssignable(const std::shared_ptr<Type> &) const noexcept {
  return nullptr;
}

std::shared_ptr<Type>
GenericInstanceValidator::isConvertible(const std::shared_ptr<Type> &) const noexcept {
  return nullptr;
}

std::shared_ptr<Type> GenericInstanceValidator::isOperable(const std::shared_ptr<Type> &,
                                                const Token &) const noexcept {
  return nullptr;
};

} // namespace nicole
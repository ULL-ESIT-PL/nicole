#include "../../../../inc/tables/typeTable/typeValidators/enumValidator.h"

namespace nicole {

std::shared_ptr<Type>
EnumValidator::isAssignable(const std::shared_ptr<Type> &) const noexcept {
  return nullptr;
}

std::shared_ptr<Type>
EnumValidator::isConvertible(const std::shared_ptr<Type> &) const noexcept {
  return nullptr;
}

std::shared_ptr<Type> EnumValidator::isOperable(const std::shared_ptr<Type> &,
                                                const Token &) const noexcept {
  return nullptr;
};

} // namespace nicole
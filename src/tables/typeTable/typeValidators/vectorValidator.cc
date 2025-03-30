#include "../../../../inc/tables/typeTable/typeValidators/vectorValidator.h"

namespace nicole {

std::shared_ptr<Type>
VectorValidator::isAssignable(const std::shared_ptr<Type> &) const noexcept {
  return nullptr;
}

std::shared_ptr<Type>
VectorValidator::isConvertible(const std::shared_ptr<Type> &) const noexcept {
  return nullptr;
}

std::shared_ptr<Type>
VectorValidator::isOperable(const std::shared_ptr<Type> &,
                            const Token &) const noexcept {
  return nullptr;
};

} // namespace nicole
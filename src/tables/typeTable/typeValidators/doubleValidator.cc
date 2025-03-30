#include "../../../../inc/tables/typeTable/typeValidators/doubleValidator.h"

namespace nicole {

std::shared_ptr<Type>
DoubleValidator::isAssignable(const std::shared_ptr<Type> & type) const noexcept {
  if (const auto basicType{std::dynamic_pointer_cast<BasicType>(type)}) {
    switch (basicType->baseKind()) {
    case BasicKind::Double:
    case BasicKind::Int:
    case BasicKind::Bool:
    case BasicKind::Float:
      return std::make_shared<BasicType>(BasicKind::Double);
    default:
      return nullptr;
    }
  } else if (const auto constType{std::dynamic_pointer_cast<ConstType>(type)}) {
    if (const auto basicType2{
            std::dynamic_pointer_cast<BasicType>(constType->baseType())}) {
      switch (basicType2->baseKind()) {
      case BasicKind::Double:
      case BasicKind::Int:
      case BasicKind::Bool:
      case BasicKind::Float:
        return std::make_shared<BasicType>(BasicKind::Double);
      default:
        return nullptr;
      }
    }
  }
  return nullptr;
}

std::shared_ptr<Type>
DoubleValidator::isConvertible(const std::shared_ptr<Type> &) const noexcept {
  return nullptr;
}

std::shared_ptr<Type>
DoubleValidator::isOperable(const std::shared_ptr<Type> &,
                            const Token &) const noexcept {
  return nullptr;
};

} // namespace nicole
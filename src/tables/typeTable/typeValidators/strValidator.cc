#include "../../../../inc/tables/typeTable/typeValidators/strValidator.h"

namespace nicole {

std::shared_ptr<Type>
StrValidator::isAssignable(const std::shared_ptr<Type> &type) const noexcept {
    if (const auto basicType{std::dynamic_pointer_cast<BasicType>(type)}) {
    switch (basicType->baseKind()) {
    case BasicKind::Str:
    case BasicKind::Char:
      return std::make_shared<BasicType>(BasicKind::Str);
    default:
      return nullptr;
    }
  } else if (const auto constType{std::dynamic_pointer_cast<ConstType>(type)}) {
    if (const auto basicType2{
            std::dynamic_pointer_cast<BasicType>(constType->baseType())}) {
      switch (basicType2->baseKind()) {
      case BasicKind::Str:
      case BasicKind::Char:
        return std::make_shared<BasicType>(BasicKind::Str);
      default:
        return nullptr;
      }
    }
  }
  return nullptr;
}

std::shared_ptr<Type>
StrValidator::isConvertible(const std::shared_ptr<Type> &) const noexcept {
  return nullptr;
}

std::shared_ptr<Type> StrValidator::isOperable(const std::shared_ptr<Type> &,
                                                const Token &) const noexcept {
  return nullptr;
};

} // namespace nicole
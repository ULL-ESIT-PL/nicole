#include "../../../../inc/tables/typeTable/typeValidators/charValidator.h"

namespace nicole {

std::shared_ptr<Type>
CharValidator::isAssignable(const std::shared_ptr<Type> &type) const noexcept {
  if (const auto basicType{std::dynamic_pointer_cast<BasicType>(type)}) {
    switch (basicType->baseKind()) {
    case BasicKind::Char:
    case BasicKind::Int:
    case BasicKind::Bool:
      return std::make_shared<BasicType>(BasicKind::Char);
    default:
      return nullptr;
    }
  } else if (const auto constType{std::dynamic_pointer_cast<ConstType>(type)}) {
    if (const auto basicType2{
            std::dynamic_pointer_cast<BasicType>(constType->baseType())}) {
      switch (basicType2->baseKind()) {
      case BasicKind::Char:
      case BasicKind::Int:
      case BasicKind::Bool:
        return std::make_shared<BasicType>(BasicKind::Char);
      default:
        return nullptr;
      }
    }
  }
  return nullptr;
}

std::shared_ptr<Type>
CharValidator::isConvertible(const std::shared_ptr<Type> &) const noexcept {
  return nullptr;
}

std::shared_ptr<Type> CharValidator::isOperable(const std::shared_ptr<Type> &,
                                                const Token &) const noexcept {
  return nullptr;
};

} // namespace nicole
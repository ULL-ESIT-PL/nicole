#include "../../../../inc/tables/typeTable/typeValidators/nullValidator.h"

namespace nicole {

std::shared_ptr<Type>
NullValidator::isAssignable(const std::shared_ptr<Type> &) const noexcept {
  return nullptr;
}

std::shared_ptr<Type>
NullValidator::isConvertible(const std::shared_ptr<Type> &) const noexcept {
  return nullptr;
}

std::shared_ptr<Type>
NullValidator::isOperable(const std::shared_ptr<Type> &type,
                          const Token &tk) const noexcept {
  auto ptr = std::dynamic_pointer_cast<PointerType>(type);
  if (!ptr) {
    if (auto ctype = std::dynamic_pointer_cast<ConstType>(type)) {
      ptr = std::dynamic_pointer_cast<PointerType>(ctype->baseType());
    }
  }
  if (ptr) {
    switch (tk.type()) {
    case TokenType::EQUAL:
    case TokenType::NOTEQUAL:
      return std::make_shared<BasicType>(BasicKind::Bool);
    default:
      break;
    }
  }
  return nullptr;
};

} // namespace nicole
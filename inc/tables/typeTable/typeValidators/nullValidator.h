#ifndef NULL_VALIDATOR_H
#define NULL_VALIDATOR_H

#include "baseTypeValidator.h"
#include <memory>

namespace nicole {

class NullValidator final : public BaseTypeValidator {
public:
  [[nodiscard]] std::shared_ptr<Type>
  isAssignable(const std::shared_ptr<Type> &) const noexcept override {
    return nullptr;
  }

  [[nodiscard]] std::shared_ptr<Type>
  isConvertible(const std::shared_ptr<Type> &) const noexcept override {
    return nullptr;
  }

  [[nodiscard]] std::shared_ptr<Type>
  isOperable(const std::shared_ptr<Type> &type,
             const Token &tk) const noexcept override {
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
};

} // namespace nicole

#endif
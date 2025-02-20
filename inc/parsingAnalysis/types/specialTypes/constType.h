#ifndef CONST_TYPE_H
#define CONST_TYPE_H

#include "../type.h"
#include <memory>
#include <string>

namespace nicole {

class ConstType final : public Type {
  std::shared_ptr<Type> baseType_;

public:
  explicit ConstType(const std::shared_ptr<Type> &baseType) noexcept
      : baseType_{baseType} {}

  [[nodiscard]] const std::shared_ptr<Type> &baseType() const noexcept {
    return baseType_;
  }

  [[nodiscard]] std::string toString() const noexcept override {
    return "const " + baseType_->toString();
  }
};

} // namespace nicole

#endif
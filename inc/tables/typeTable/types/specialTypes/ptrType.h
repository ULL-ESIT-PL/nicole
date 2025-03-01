#ifndef POINTER_TYPE_H
#define POINTER_TYPE_H

#include "../type.h"
#include <memory>
#include <string>

namespace nicole {

class PointerType final : public Type {
private:
  std::shared_ptr<Type> baseType_;

public:
  explicit PointerType(const std::shared_ptr<Type> &baseType)
      : baseType_{baseType} {}

  [[nodiscard]] const std::shared_ptr<Type> &baseType() const noexcept {
    return baseType_;
  }

  [[nodiscard]] std::string toString() const noexcept override {
    return baseType_->toString() + "*";
  }
};

} // namespace nicole

#endif
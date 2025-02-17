#ifndef POINTER_TYPE_H
#define POINTER_TYPE_H

#include "../type.h"
#include <memory>
#include <string>

namespace nicole {

class PointerType : public Type {
  std::unique_ptr<Type> baseType;

public:
  explicit PointerType(std::unique_ptr<Type> base)
      : baseType(std::move(base)) {}
      
  [[nodiscard]] std::string toString() const noexcept override {
    return baseType->toString() + "*";
  }
};

} // namespace nicole

#endif
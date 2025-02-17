#ifndef CONST_TYPE_H
#define CONST_TYPE_H

#include "../type.h"
#include <memory>
#include <string>

namespace nicole {

class ConstType : public Type {
  std::unique_ptr<Type> baseType;

public:
  explicit ConstType(std::unique_ptr<Type> base) : baseType(std::move(base)) {}
  
  [[nodiscard]] std::string toString() const noexcept override {
    return "const " + baseType->toString();
  }
};

} // namespace nicole

#endif
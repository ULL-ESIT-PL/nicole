#ifndef VECTOR_TYPE_H
#define VECTOR_TYPE_H

#include "../type.h"
#include <memory>
#include <string>

namespace nicole {

class VectorType : public Type {
  std::unique_ptr<Type> elementType;

public:
  explicit VectorType(std::unique_ptr<Type> elem)
      : elementType(std::move(elem)) {}
      
  [[nodiscard]] std::string toString() const noexcept override {
    return "vector<" + elementType->toString() + ">";
  }
};

} // namespace nicole

#endif
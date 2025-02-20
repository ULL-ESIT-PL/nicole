#ifndef VECTOR_TYPE_H
#define VECTOR_TYPE_H

#include "../type.h"
#include <memory>
#include <string>

namespace nicole {

class VectorType final : public Type {
  std::shared_ptr<Type> elementType_;

public:
  explicit VectorType(const std::shared_ptr<Type>& elementType)
      : elementType_{elementType} {}

  [[nodiscard]] const std::shared_ptr<Type> &elementType() const noexcept {
    return elementType_;
  }
      
  [[nodiscard]] std::string toString() const noexcept override {
    return "vector<" + elementType_->toString() + ">";
  }
};

} // namespace nicole

#endif
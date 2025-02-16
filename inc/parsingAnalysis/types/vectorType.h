#ifndef VECTOR_TYPE_H
#define VECTOR_TYPE_H

#include "type.h"
#include <memory>

namespace nicole {

class VectorType final : public Type {
private:
  std::shared_ptr<Type> storedType_;

public:
  explicit VectorType(const std::shared_ptr<Type> storedType,
                      const bool isConst) noexcept
      : Type{"vector", isConst}, storedType_{storedType} {}

  [[nodiscard]] const std::shared_ptr<Type>& storedType() const noexcept { return storedType_; }
};

} // namespace nicole

#endif
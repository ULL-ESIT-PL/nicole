#ifndef VECTOR_TYPE_H
#define VECTOR_TYPE_H

#include "genericType.h"
#include <memory>

namespace nicole {

class VectorType final : public GenericType {
private:
  std::shared_ptr<GenericType> storedType_;

public:
  explicit VectorType(const std::shared_ptr<GenericType> storedType,
                      const bool isConst) noexcept
      : GenericType{"vector", isConst}, storedType_{storedType} {}

  [[nodiscard]] const std::shared_ptr<GenericType> &
  storedType() const noexcept {
    return storedType_;
  }
};

} // namespace nicole

#endif
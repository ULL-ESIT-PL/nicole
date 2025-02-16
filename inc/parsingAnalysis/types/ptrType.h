#ifndef PTR_TYPE_H
#define PTR_TYPE_H

#include "genericType.h"
#include <memory>

namespace nicole {

class PtrType final : public GenericType {
private:
  std::shared_ptr<GenericType> storedType_;

public:
  explicit PtrType(const std::shared_ptr<GenericType>& storedType,
                      const bool isConst) noexcept
      : GenericType{"ptr", isConst}, storedType_{storedType} {}

  [[nodiscard]] const std::shared_ptr<GenericType> &storedType() const noexcept {
    return storedType_;
  }
};

} // namespace nicole

#endif
#ifndef PTR_TYPE_H
#define PTR_TYPE_H

#include "type.h"
#include <memory>

namespace nicole {

class PtrType final : public Type {
private:
  std::shared_ptr<Type> storedType_;

public:
  explicit PtrType(const std::shared_ptr<Type>& storedType,
                      const bool isConst) noexcept
      : Type{"ptr", isConst}, storedType_{storedType} {}

  [[nodiscard]] const std::shared_ptr<Type> &storedType() const noexcept {
    return storedType_;
  }
};

} // namespace nicole

#endif
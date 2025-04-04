#ifndef PLACE_HOLDER_H
#define PLACE_HOLDER_H

#include "../type.h"
#include <memory>
#include <string>

// car<T, Q>
// car<int, str> --> car_int_str
namespace nicole {

class PlaceHolder final : public Type {
private:
  std::shared_ptr<Type> genericCompound_;

public:
  explicit PlaceHolder(const std::shared_ptr<Type> &genericCompound) noexcept
      : genericCompound_(genericCompound) {}

  [[nodiscard]] std::string toString() const noexcept override {
    return "PlaceHolder: " + genericCompound_->toString();
  }

  [[nodiscard]] const std::shared_ptr<Type> &
  getGenericCompound() const noexcept {
    return genericCompound_;
  }
};

} // namespace nicole

#endif

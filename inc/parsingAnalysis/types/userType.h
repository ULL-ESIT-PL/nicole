#ifndef USER_TYPE_H
#define USER_TYPE_H

#include "type.h"
#include <memory>

namespace nicole {

class UserType final : public Type {
private:
  std::shared_ptr<Type> father_{nullptr};
  std::vector<std::shared_ptr<Type>> storedTypes_;

public:
  explicit UserType(const std::string &id,
                    const std::vector<std::shared_ptr<Type>> &storedTypes,
                    const bool isConst) noexcept
      : Type{id, isConst}, storedTypes_{storedTypes} {}

  [[nodiscard]] const std::shared_ptr<Type> &father() const noexcept {
    return father_;
  }

  [[nodiscard]] const std::vector<std::shared_ptr<Type>> &
  storedTypes() const noexcept {
    return storedTypes_;
  }
};

} // namespace nicole

#endif
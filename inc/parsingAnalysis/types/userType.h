#ifndef USER_TYPE_H
#define USER_TYPE_H

#include "genericType.h"
#include <memory>
#include <vector>

namespace nicole {

class UserType final : public GenericType {
private:
  std::shared_ptr<GenericType> father_{nullptr};
  std::vector<std::shared_ptr<GenericType>> storedTypes_;

public:
  explicit UserType(const std::string &id,
                    const std::vector<std::shared_ptr<GenericType>> &storedTypes,
                    const bool isConst) noexcept
      : GenericType{id, isConst}, storedTypes_{storedTypes} {}

  [[nodiscard]] const std::shared_ptr<GenericType> &father() const noexcept {
    return father_;
  }

  [[nodiscard]] const std::vector<std::shared_ptr<GenericType>> &
  storedTypes() const noexcept {
    return storedTypes_;
  }
};

} // namespace nicole

#endif
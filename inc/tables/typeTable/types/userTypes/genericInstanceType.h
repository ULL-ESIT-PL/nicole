#ifndef GENERIC_INSTANCE_TYPE_TYPE_H
#define GENERIC_INSTANCE_TYPE_TYPE_H

#include "userType.h"
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace nicole {

class GenericInstanceType final : public UserType {
private:
  std::vector<std::shared_ptr<Type>> typeArgs_;

public:
  GenericInstanceType(const std::shared_ptr<UserType> &genericType,
                      const std::vector<std::shared_ptr<Type>> &args) noexcept
      : UserType(genericType->name(), genericType->baseType(),
                 genericType->genericParams()),
        typeArgs_{args} {}

  [[nodiscard]] const std::vector<std::shared_ptr<Type>> &
  typeArgs() const noexcept {
    return typeArgs_;
  }

  [[nodiscard]] std::string toString() const noexcept override {
    std::ostringstream oss;
    oss << UserType::toString() << "<";
    for (size_t i = 0; i < typeArgs_.size(); ++i) {
      oss << typeArgs_[i]->toString();
      if (i != typeArgs_.size() - 1)
        oss << ", ";
    }
    oss << ">";
    return oss.str();
  }
};

} // namespace nicole

#endif

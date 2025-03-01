#ifndef GENERIC_INSTANCE_TYPE_TYPE_H
#define GENERIC_INSTANCE_TYPE_TYPE_H

#include "userType.h"
#include <memory>
#include <string>
#include <vector>

namespace nicole {

class GenericInstanceType final : public Type {
private:
  std::shared_ptr<UserType> genericType_;
  std::vector<std::shared_ptr<Type>> typeArgs_;

public:
  GenericInstanceType(const std::shared_ptr<UserType> &genType,
                      const std::vector<std::shared_ptr<Type>> &args) noexcept
      : genericType_{genType}, typeArgs_{args} {}

  [[nodiscard]] std::string toString() const noexcept override {
    std::ostringstream oss;
    oss << genericType_->toString() << "(";
    for (size_t i = 0; i < typeArgs_.size(); ++i) {
      oss << typeArgs_[i]->toString();
      if (i != typeArgs_.size() - 1)
        oss << ", ";
    }
    oss << ")";
    return oss.str();
  }
};

} // namespace nicole

#endif
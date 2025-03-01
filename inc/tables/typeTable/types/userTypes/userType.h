#ifndef USER_TYPE_H
#define USER_TYPE_H

#include "attribute.h"
#include "methodTable.h"
#include "genericParameter.h"
#include <memory>
#include <sstream>
#include <string>
#include <set>
#include <vector>

namespace nicole {

class UserType final : public Type {
private:
  std::string name_;
  std::shared_ptr<UserType> baseType_; // Solo se permite una base
  std::vector<GenericParameter> genericParams_;
  std::set<Attribute> attributes_;
  MethodTable methodTable_{};

public:
  explicit UserType(const std::string &name,
                    const std::shared_ptr<UserType> &baseType,
                    const std::vector<GenericParameter> &genericParams) noexcept
      : name_{name}, baseType_(baseType), genericParams_{genericParams} {}

  [[nodiscard]] const std::string &name() const noexcept { return name_; }

  [[nodiscard]] const std::shared_ptr<UserType> &baseType() const noexcept {
    return baseType_;
  }

  [[nodiscard]] const std::vector<GenericParameter> &
  genericParams() const noexcept {
    return genericParams_;
  }

  [[nodiscard]] std::string toString() const noexcept override {
    std::ostringstream oss;
    oss << name_;
    if (!genericParams_.empty()) {
      oss << "<";
      for (size_t i = 0; i < genericParams_.size(); ++i) {
        oss << genericParams_[i].name();
        if (i != genericParams_.size() - 1)
          oss << ", ";
      }
      oss << ">";
    }
    if (baseType_) {
      oss << " : " << baseType_->toString();
    }
    return oss.str();
  }
};

} // namespace nicole

#endif
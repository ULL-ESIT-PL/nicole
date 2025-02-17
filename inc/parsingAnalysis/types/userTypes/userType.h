#ifndef USER_TYPE_H
#define USER_TYPE_H

#include "../type.h"
#include "genericParameter.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <sstream>

namespace nicole {

class UserType : public Type {
  std::string name;
  std::shared_ptr<UserType> baseType; // Solo se permite una base
  std::vector<GenericParameter> genericParams;

public:
  explicit UserType(std::string n) : name(std::move(n)), baseType(nullptr) {}

  void setBase(const std::shared_ptr<UserType> &base) {
    if (baseType != nullptr) {
      std::cerr << "Error: La herencia múltiple no está permitida."
                << std::endl;
      return;
    }
    baseType = base;
  }

  void addGenericParam(GenericParameter param) {
    genericParams.push_back(param);
  }

  [[nodiscard]] std::string toString() const noexcept override {
    std::ostringstream oss;
    oss << name;
    if (!genericParams.empty()) {
      oss << "<";
      for (size_t i = 0; i < genericParams.size(); ++i) {
        oss << genericParams[i].toString();
        if (i != genericParams.size() - 1)
          oss << ", ";
      }
      oss << ">";
    }
    if (baseType) {
      oss << " : " << baseType->toString();
    }
    return oss.str();
  }
};

} // namespace nicole

#endif
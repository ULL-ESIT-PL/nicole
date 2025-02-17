#ifndef GENERIC_INSTANCE_TYPE_TYPE_H
#define POINTER_TYPE_H

#include "userType.h"
#include <memory>
#include <string>
#include <vector>

namespace nicole {

class GenericInstanceType : public Type {
  std::unique_ptr<UserType> genericType;
  std::vector<std::unique_ptr<Type>> typeArgs;

public:
  GenericInstanceType(std::unique_ptr<UserType> genType,
                      std::vector<std::unique_ptr<Type>> args)
      : genericType(std::move(genType)), typeArgs(std::move(args)) {}

  [[nodiscard]] std::string toString() const noexcept override {
    std::ostringstream oss;
    oss << genericType->toString() << "(";
    for (size_t i = 0; i < typeArgs.size(); ++i) {
      oss << typeArgs[i]->toString();
      if (i != typeArgs.size() - 1)
        oss << ", ";
    }
    oss << ")";
    return oss.str();
  }
};

} // namespace nicole

#endif
#ifndef VOID_TYPE_H
#define VOID_TYPE_H

#include "../type.h"
#include <string>

namespace nicole {

class VoidType : public Type {
public:
  [[nodiscard]] std::string toString() const noexcept override {
    return "void";
  }
};

} // namespace nicole

#endif
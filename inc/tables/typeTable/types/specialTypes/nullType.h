#ifndef NULL_TYPE_H
#define NULL_TYPE_H

#include "../type.h"
#include <string>

namespace nicole {

class NullType final : public Type {
public:
  [[nodiscard]] std::string toString() const noexcept override {
    return "nullptr";
  }
};

} // namespace nicole

#endif
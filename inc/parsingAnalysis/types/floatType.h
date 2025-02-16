#ifndef FLOAT_TYPE_H
#define FLOAT_TYPE_H

#include "type.h"

namespace nicole {

class FloatType final : public Type {
public:
  explicit FloatType(const bool isConst) noexcept : Type{"float", isConst} {}
};

} // namespace nicole

#endif
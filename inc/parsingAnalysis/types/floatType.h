#ifndef FLOAT_TYPE_H
#define FLOAT_TYPE_H

#include "genericType.h"

namespace nicole {

class FloatType final : public GenericType {
public:
  explicit FloatType(const bool isConst) noexcept : GenericType{"float", isConst} {}
};

} // namespace nicole

#endif
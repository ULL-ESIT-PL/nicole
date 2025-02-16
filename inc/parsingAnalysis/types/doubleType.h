#ifndef DOUBLE_TYPE_H
#define DOUBLE_TYPE_H

#include "genericType.h"

namespace nicole {

class DoubleType final : public GenericType {
public:
  explicit DoubleType(const bool isConst) noexcept : GenericType{"double", isConst} {}
};

} // namespace nicole

#endif
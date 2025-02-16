#ifndef DOUBLE_TYPE_H
#define DOUBLE_TYPE_H

#include "type.h"

namespace nicole {

class DoubleType final : public Type {
public:
  explicit DoubleType(const bool isConst) noexcept : Type{"double", isConst} {}
};

} // namespace nicole

#endif
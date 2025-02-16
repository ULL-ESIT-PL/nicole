#ifndef INT_TYPE_H
#define INT_TYPE_H

#include "type.h"

namespace nicole {

class IntType final : public Type {
public:
  explicit IntType(const bool isConst) noexcept : Type{"int", isConst} {}
};

} // namespace nicole

#endif
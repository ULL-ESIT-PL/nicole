#ifndef NULL_TYPE_H
#define NULL_TYPE_H

#include "type.h"

namespace nicole {

class NullType final : public Type {
public:
  explicit NullType(const bool isConst) noexcept : Type{"null", isConst} {}
};

} // namespace nicole

#endif
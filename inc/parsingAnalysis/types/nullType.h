#ifndef NULL_TYPE_H
#define NULL_TYPE_H

#include "genericType.h"

namespace nicole {

class NullType final : public GenericType {
public:
  explicit NullType(const bool isConst) noexcept : GenericType{"null", isConst} {}
};

} // namespace nicole

#endif
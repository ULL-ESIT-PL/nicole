#ifndef INT_TYPE_H
#define INT_TYPE_H

#include "genericType.h"

namespace nicole {

class IntType final : public GenericType {
public:
  explicit IntType(const bool isConst) noexcept : GenericType{"int", isConst} {}
};

} // namespace nicole

#endif
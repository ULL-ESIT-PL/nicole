#ifndef BOOL_TYPE_H
#define BOOL_TYPE_H

#include "genericType.h"

namespace nicole {

class BoolType final : public GenericType {
public:
  explicit BoolType(const bool isConst) noexcept
      : GenericType{"bool", isConst} {}
};

} // namespace nicole

#endif
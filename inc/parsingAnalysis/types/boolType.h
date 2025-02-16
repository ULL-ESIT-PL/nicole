#ifndef BOOL_TYPE_H
#define BOOL_TYPE_H

#include "type.h"

namespace nicole {

class BoolType final : public Type {
public:
  explicit BoolType(const bool isConst) noexcept : Type{"bool", isConst} {}
};

} // namespace nicole

#endif
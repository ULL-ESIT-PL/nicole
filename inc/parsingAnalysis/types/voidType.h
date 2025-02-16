#ifndef VOID_TYPE_H
#define VOID_TYPE_H

#include "type.h"

namespace nicole {

class VoidType final : public Type {
public:
  explicit VoidType(const bool isConst) noexcept : Type{"void", isConst} {}
};

} // namespace nicole

#endif
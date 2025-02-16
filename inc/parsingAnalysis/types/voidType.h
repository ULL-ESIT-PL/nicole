#ifndef VOID_TYPE_H
#define VOID_TYPE_H

#include "genericType.h"

namespace nicole {

class VoidType final : public GenericType {
public:
  explicit VoidType(const bool isConst) noexcept : GenericType{"void", isConst} {}
};

} // namespace nicole

#endif
#ifndef STR_TYPE_H
#define STR_TYPE_H

#include "type.h"

namespace nicole {

class StrType final : public Type {
public:
  explicit StrType(const bool isConst) noexcept : Type{"str", isConst} {}
};

} // namespace nicole

#endif
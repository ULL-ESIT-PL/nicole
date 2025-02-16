#ifndef STR_TYPE_H
#define STR_TYPE_H

#include "genericType.h"

namespace nicole {

class StrType final : public GenericType {
public:
  explicit StrType(const bool isConst) noexcept : GenericType{"str", isConst} {}
};

} // namespace nicole

#endif
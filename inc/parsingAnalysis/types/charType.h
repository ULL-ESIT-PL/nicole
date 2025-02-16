#ifndef CHAR_TYPE_H
#define CHAR_TYPE_H

#include "type.h"

namespace nicole {

class CharType final : public Type {
public:
  explicit CharType(const bool isConst) noexcept : Type{"char", isConst} {}
};

} // namespace nicole

#endif
#ifndef CHAR_TYPE_H
#define CHAR_TYPE_H

#include "genericType.h"

namespace nicole {

class CharType final : public GenericType {
public:
  explicit CharType(const bool isConst) noexcept : GenericType{"char", isConst} {}
};

} // namespace nicole

#endif
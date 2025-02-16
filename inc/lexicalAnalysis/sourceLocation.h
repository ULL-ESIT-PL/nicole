#ifndef SOURCE_LOCATION_H
#define SOURCE_LOCATION_H

#include "token.h"

namespace nicole {

class SourceLocation {
private:
  Token begin_;
  Token end_;

public:
  explicit SourceLocation(const Token &begin, const Token &end) noexcept
      : begin_{begin}, end_{end} {}

  [[nodiscard]] const Token &beginLoc() const noexcept { return begin_; }

  [[nodiscard]] const Token &endLoc() const noexcept { return end_; }
};

} // namespace nicole

#endif
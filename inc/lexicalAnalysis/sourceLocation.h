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

  [[nodiscard]] bool operator<(const SourceLocation &loc) const noexcept {
    if (begin_.location() < loc.begin_.location()) {
      return true;
    }
    if (end_.location() < loc.end_.location()) {
      return true;
    }
    return false;
  }
};

} // namespace nicole

#endif
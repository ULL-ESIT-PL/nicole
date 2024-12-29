#ifndef SOURCE_LOCATION_H
#define SOURCE_LOCATION_H

#include "../../lexicalAnalysis/location.h"

namespace nicole {

class SourceLocation final {
private:
  Location begin_;
  Location end_;

public:
  explicit SourceLocation(const Location &begin, const Location &end) noexcept
      : begin_{begin}, end_{end} {}

  [[nodiscard]] Location begin() const noexcept { return begin_; }

  [[nodiscard]] Location end() const noexcept { return begin_; }
};

} // namespace nicole

#endif
#ifndef BREAK_TYPE_H
#define BREAK_TYPE_H

#include "../type.h"
#include <memory>
#include <string>

namespace nicole {

class BreakType final : public Type {
public:
  [[nodiscard]] std::string toString() const noexcept override {
    return "breakType";
  }
};

} // namespace nicole

#endif
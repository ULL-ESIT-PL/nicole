#ifndef NO_PROPAGATE_TYPE_H
#define NO_PROPAGATE_TYPE_H

#include "../type.h"
#include <memory>
#include <string>

namespace nicole {

class NoPropagateType final : public Type {
public:
  [[nodiscard]] std::string toString() const noexcept override {
    return "NoPropagateType";
  }
};

} // namespace nicole

#endif
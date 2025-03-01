#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include "../../../symbol.h"
#include "../type.h"
#include <cstddef>
#include <memory>

namespace nicole {

class Attribute final : public Symbol {
private:
  std::shared_ptr<Type> type_{};
  std::size_t position_;

public:
  explicit Attribute(const std::string &id,
                     const std::shared_ptr<Type> &type, const std::size_t pos) noexcept
      : Symbol{id}, type_{type}, position_(pos) {}

  [[nodiscard]] const std::shared_ptr<Type> &type() const noexcept {
    return type_;
  }

  [[nodiscard]] const std::size_t position() const noexcept { return position_; }
};

} // namespace nicole

#endif
#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include "../../../symbol.h"
#include "../type.h"
#include <memory>

namespace nicole {

class Attribute final : public Symbol {
private:
  std::shared_ptr<Type> type_{};

public:
  explicit Attribute(const std::string &id,
                     const std::shared_ptr<Type> &type) noexcept
      : Symbol{id}, type_{type} {}

  [[nodiscard]] const std::shared_ptr<Type> &type() const noexcept {
    return type_;
  }

  bool operator<(const Attribute& other) const { return id_ < other.id_; }
};

} // namespace nicole

#endif
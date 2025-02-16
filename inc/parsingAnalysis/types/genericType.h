#ifndef GENERIC_TYPE_H
#define GENERIC_TYPE_H

#include <string>

namespace nicole {

class GenericType {
private:
  std::string id_;
  bool isConst_;

public:
  explicit GenericType(const std::string &id, bool isConst) noexcept
      : id_{id}, isConst_{isConst} {}

  virtual ~GenericType() = default;

  [[nodiscard]] std::string id() const noexcept { return id_; }

  [[nodiscard]] bool isConst() const noexcept { return isConst_; }

  [[nodiscard]] bool operator<(const GenericType *type) const noexcept {
    return id_ < type->id_;
  };
};

} // namespace nicole

#endif

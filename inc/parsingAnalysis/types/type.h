#ifndef TYPE_H
#define TYPE_H

#include <memory>
#include <string_view>
#include <vector>

namespace nicole {

class Type {
private:
  std::string id_;
  bool isConst_;

public:
  explicit Type(const std::string &id, bool isConst) noexcept
      : id_{id}, isConst_{isConst} {}

  virtual ~Type() = default;

  [[nodiscard]] std::string id() const noexcept { return id_; }

  [[nodiscard]] bool isConst() const noexcept { return isConst_; }

  [[nodiscard]] bool operator==(const Type *type) const noexcept {
    return id_ == type->id_;
  };

  [[nodiscard]] bool operator<(const Type *type) const noexcept {
    return id_ < type->id_;
  };
};

} // namespace nicole

#endif

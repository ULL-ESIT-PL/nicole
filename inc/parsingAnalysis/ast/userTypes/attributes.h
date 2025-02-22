#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#include <cstddef>
#include <memory>
#include <string>
#include <vector>
#include "../../types/type.h"

namespace nicole {

class Attributes {
private:
  std::vector<std::pair<std::string, std::shared_ptr<Type>>> params_;

public:
  explicit Attributes(
      const std::vector<std::pair<std::string, std::shared_ptr<Type>>> &params) noexcept
      : params_{params} {}

  [[nodiscard]] const std::vector<std::pair<std::string, std::shared_ptr<Type>>> &
  params() const noexcept {
    return params_;
  }

  [[nodiscard]] std::size_t size() const noexcept { return params_.size(); }

  auto begin() const noexcept { return params_.begin(); }

  auto end() const noexcept { return params_.end(); }
};

} // namespace nicole

#endif
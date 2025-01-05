#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#include <cstddef>
#include <string>
#include <vector>

namespace nicole {

class Attributes {
private:
  std::vector<std::pair<std::string, std::string>> params_;

public:
  explicit Attributes(
      const std::vector<std::pair<std::string, std::string>> &params) noexcept
      : params_{params} {}

  [[nodiscard]] const std::vector<std::pair<std::string, std::string>> &
  params() const noexcept {
    return params_;
  }

  [[nodiscard]] std::size_t size() const noexcept { return params_.size(); }

  auto begin() const noexcept { return params_.begin(); }

  auto end() const noexcept { return params_.end(); }
};

} // namespace nicole

#endif
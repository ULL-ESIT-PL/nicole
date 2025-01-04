#ifndef LOCATION_H
#define LOCATION_H

#include <filesystem>

namespace nicole {

// Represents the position of the token
class Location final {
private:
  std::filesystem::path file_;
  std::size_t row_;
  std::size_t col_;

public:
  explicit Location(const std::filesystem::path &file, std::size_t row,
                    std::size_t col) noexcept
      : file_{file}, row_{row}, col_{col} {};

  [[nodiscard]] const std::filesystem::path &file() const noexcept {
    return file_;
  }

  [[nodiscard]] std::size_t row() const noexcept { return row_; }

  [[nodiscard]] std::size_t col() const noexcept { return col_; }
};

} // namespace nicole

#endif
#ifndef LOCATION_H
#define LOCATION_H

#include <cstddef>
#include <filesystem>

namespace nicole {

// Represents the position of the token
class Location final {
private:
  std::filesystem::path file_{""};
  std::size_t row_{0};
  std::size_t col_{0};

public:
  Location(const std::filesystem::path &file, std::size_t row, std::size_t col)
      : file_{file}, row_{row}, col_{col} {};

  std::filesystem::path file() const { return file_; }

  std::size_t row() const { return row_; }

  std::size_t col() const { return col_; }
};

} // namespace nicole

#endif
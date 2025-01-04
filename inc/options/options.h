#ifndef OPTIONS_H
#define OPTIONS_H

#include <filesystem>

namespace nicole {

class Options final {
private:
  bool help_;
  bool optimize_;
  std::string binaryName_;
  std::filesystem::path entryFilePath_;

public:
  explicit Options(const bool help, const bool optimize,
                   const std::string &binaryName,
                   const std::filesystem::path &entryFilePath) noexcept
      : help_{help}, optimize_{optimize}, binaryName_{binaryName},
        entryFilePath_{entryFilePath} {}

  [[nodiscard]] bool help() const noexcept { return help_; }

  [[nodiscard]] bool optimize() const noexcept { return optimize_; }

  [[nodiscard]] const std::string &binaryName() const noexcept {
    return binaryName_;
  }

  [[nodiscard]] const std::filesystem::path &entryFilePath() const noexcept {
    return entryFilePath_;
  }
};

} // namespace nicole

#endif
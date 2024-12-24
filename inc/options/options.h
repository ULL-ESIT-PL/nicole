#ifndef OPTIONS_H
#define OPTIONS_H

#include <filesystem>

namespace nicole {

class Options final {
private:
  bool help_{false};
  bool optimize_{false};
  std::string binaryName_{""};
  std::filesystem::path entryFilePath_{""};

public:
  explicit Options(const bool help, const bool optimize,
                   const std::string &binaryName,
                   const std::filesystem::path &entryFilePath) noexcept
      : help_{help}, optimize_{optimize}, binaryName_{binaryName},
        entryFilePath_{entryFilePath} {}

  [[nodiscard]] bool help() const { return help_; }

  [[nodiscard]] bool optimize() const { return optimize_; }

  [[nodiscard]] std::string binaryName() const { return binaryName_; }

  [[nodiscard]] std::filesystem::path entryFilePath() const {
    return entryFilePath_;
  }
};

} // namespace nicole

#endif
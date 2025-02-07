#ifndef OPTIONS_H
#define OPTIONS_H

#include <filesystem>

namespace nicole {

class Options final {
private:
  bool help_;
  bool optimize_;
  bool printTree_;
  bool printIR_;
  bool validateTree_;
  std::string binaryName_;
  std::filesystem::path entryFilePath_;

public:
  explicit Options(const bool help, const bool optimize, const bool printTree,
                   const bool printIR, const bool validateTree,
                   const std::string &binaryName,
                   const std::filesystem::path &entryFilePath) noexcept
      : help_{help}, optimize_{optimize}, printTree_{printTree},
        printIR_{printIR}, validateTree_{validateTree}, binaryName_{binaryName},
        entryFilePath_{entryFilePath} {}

  [[nodiscard]] bool help() const noexcept { return help_; }

  [[nodiscard]] bool optimize() const noexcept { return optimize_; }

  [[nodiscard]] bool printTree() const noexcept { return printTree_; }

  [[nodiscard]] bool printIR() const noexcept { return printIR_; }

  [[nodiscard]] bool validateTree() const noexcept { return validateTree_; }

  [[nodiscard]] const std::string &binaryName() const noexcept {
    return binaryName_;
  }

  [[nodiscard]] const std::filesystem::path &entryFilePath() const noexcept {
    return entryFilePath_;
  }
};

} // namespace nicole

#endif
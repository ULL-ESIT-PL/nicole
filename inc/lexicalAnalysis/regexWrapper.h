#ifndef REGEX_NC_H
#define REGEX_NC_H

#include <regex>
#include <string>

namespace nicole {

// Regex doesn´t store a raw string of the pattern so a wrapper is need like the
// one used in boost library
class RegexWrapper final {
private:
  /* data */
  std::string rawPattern_{""};
  std::regex pattern_{""};

public:
  explicit RegexWrapper(const std::string &rawPattern) noexcept
      : rawPattern_{rawPattern},
        pattern_{rawPattern, std::regex_constants::optimize} {};

  [[nodiscard]] std::string str() const { return rawPattern_; }

  [[nodiscard]] std::regex pattern() const { return pattern_; }

  [[nodiscard]] bool match(const std::string &str) const {
    return std::regex_match(str, pattern_);
  }
};

} // namespace nicole

#endif
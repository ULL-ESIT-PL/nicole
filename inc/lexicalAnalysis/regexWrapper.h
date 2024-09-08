#ifndef REGEX_NC_H
#define REGEX_NC_H

#include <regex>
#include <string>

namespace nicole {
class RegexWrapper {
 private:
  /* data */
  std::string rawPattern_{""};
  std::regex pattern_{""};

 public:
  RegexWrapper(const std::string& rawPattern)
      : rawPattern_{rawPattern},
        pattern_{rawPattern, std::regex_constants::optimize} {};
  std::string str() const { return rawPattern_; }
  std::regex pattern() const { return pattern_; }
  bool match(const std::string& str) const { return std::regex_match(str, pattern_); }
};
}  // namespace nicole

#endif
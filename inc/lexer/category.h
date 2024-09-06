#ifndef CATEGORY_H
#define CATEGORY_H

#include <boost/regex.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "token.h"

namespace nicole {

class Category final {
 private:
  boost::regex pattern_{""};
  bool skip_{false};

 public:
  Category(const std::string& matcher, const bool skip)
      : pattern_{matcher, boost::regex_constants::optimize}, skip_{skip} {};

  boost::regex pattern() const { return pattern_; }
  std::string rawPattern() const { return pattern_.str(); }
  bool skip() const { return skip_; }

  // defined in CATEGORY.cc
  std::string name() const;
  bool matchToken(const Token& token) const;
};

}  // namespace nicole

#endif
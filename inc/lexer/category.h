#ifndef CATEGORY_H
#define CATEGORY_H

#include <boost/regex.hpp>
#include <stdexcept>

#include "token.h"
using namespace std;

namespace nicole {

class Category {
 private:
  TokenType type_{TokenType::UNDEFINED};
  boost::regex pattern_{};

 public:
  Category(const TokenType& type, const string& pattern)
      : type_{type}, pattern_{pattern, boost::regex_constants::optimize} {};
  TokenType type() const { return type_; }
  std::string rawPattern() const { return pattern_.str(); }
  bool match(const std::string& str) const { return boost::regex_match(str, pattern_); }
  bool match(const Token& tk) const { return boost::regex_match(tk.raw(), pattern_); }
};

}  // namespace nicole

#endif

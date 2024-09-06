#include "../../inc/lexer/category.h"

namespace nicole {
std::string Category::name() const {
  const std::string RAW{rawPattern()};
  const char LESS{'<'};
  const char GREATER{'>'};
  const int FIRST_POS_AFTER_SMALLER = RAW.find_first_of(LESS) + 1;
  const int FIRST_POS_BEFORE_GREATER = RAW.find_first_of(GREATER);
  return RAW.substr(FIRST_POS_AFTER_SMALLER,
                    FIRST_POS_BEFORE_GREATER - FIRST_POS_AFTER_SMALLER);
}

bool Category::matchToken(const Token& token) const {
  return regex_match(token.raw(), pattern_);
}
}  // namespace nicole
#ifndef LEXER_H
#define LEXER_H

#include <set>
#include <vector>
#include <filesystem>

#include "category.h"
using namespace std;

namespace nicole {

class Lexer {
 private:
  set<Category> categories_{};
 public:
  Lexer(const set<Category>& categories): categories_{categories} {};
  std::vector<Token> run(const filesystem::path& path) const;
};

}  // namespace nicole

#endif

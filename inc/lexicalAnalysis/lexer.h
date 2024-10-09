#ifndef LEXER_H
#define LEXER_H

#include <llvm/Support/ErrorHandling.h>

#include <filesystem>
#include <fstream>
#include <vector>

#include "category.h"

namespace nicole {

// Lexer that tokenize an input file
class Lexer final {
private:
  std::vector<Category> categories_{};

  // makes a category that matches with every token of our sintax
  Category concatCategories() const;

  std::string readFile(const std::filesystem::path &fileName) const;

  void checkUnmatched(const std::vector<Token> &tokens) const;

public:
  Lexer(const std::vector<Category> &categories);

  std::vector<Token> analyze(const std::filesystem::path &fileName,
                             bool verbose = false) const;
};

} // namespace nicole

#endif
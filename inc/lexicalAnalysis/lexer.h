#ifndef LEXER_H
#define LEXER_H

#include <llvm/Support/ErrorHandling.h>

#include <filesystem>
#include <fstream>
#include <vector>

#include "category.h"
#include "tokenStream.h"

namespace nicole {

// Lexer that tokenize an input file
class Lexer final {
private:
  std::vector<Category> categories_{};

  // makes a category that matches with every token of our sintax
  [[nodiscard]] Category concatCategories() const;

  [[nodiscard]] std::expected<std::string, Error>
  readFile(const std::filesystem::path &fileName) const;

  [[nodiscard]] std::expected<void, Error>
  checkUnmatched(const std::vector<Token> &tokens) const;

public:
  explicit Lexer(const std::vector<Category> &categories) noexcept
      : categories_{categories} {};

  [[nodiscard]] std::vector<Category> categories() const { return categories_; }

  [[nodiscard]] std::expected<TokenStream, Error>
  analyze(const std::filesystem::path &fileName, bool verbose = false) const;
};

} // namespace nicole

#endif
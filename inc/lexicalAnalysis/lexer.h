#ifndef LEXER_H
#define LEXER_H

#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#include "category.h"
#include "tokenStream.h"

namespace nicole {

// Lexer that tokenize an input file
class Lexer final {
private:
  std::vector<Category> categories_{};

  // makes a category that matches with every token of our sintax
  [[nodiscard]] const Category concatCategories() const noexcept;

  [[nodiscard]] const std::expected<std::string, Error>
  readFile(const std::filesystem::path &fileName) const noexcept;

  [[nodiscard]] const std::expected<void, Error>
  checkUnmatched(const std::vector<Token> &tokens) const noexcept;

public:
  explicit Lexer(const std::vector<Category> &categories) noexcept
      : categories_{categories} {};

  [[nodiscard]] const std::vector<Category> &categories() const noexcept {
    return categories_;
  }

  [[nodiscard]] const std::expected<TokenStream, Error>
  analyze(const std::filesystem::path &fileName,
          bool verbose = false) const noexcept;
};

} // namespace nicole

#endif
#include "../../inc/lexicalAnalysis/lexer.h"
#include <cstddef>

namespace nicole {

Lexer::Lexer(const std::vector<Category> &categories)
    : categories_{categories} {}

Category Lexer::concatCategories() const {
  std::string pattern{""};
  for (const auto &category : categories_) {
    pattern += category.rawPattern() + "|";
  }
  pattern.pop_back();
  return Category{TokenType::ALL, pattern, false};
}

void Lexer::checkUnmatched(const std::vector<Token> &tokens) const {
  bool unmatchedFlag{false};
  std::string everyUnmatched{"Unmatched tokens:\n"};
  for (const auto &TOKEN : tokens) {
    if (TOKEN.type() == TokenType::UNMATCHED) {
      everyUnmatched += TOKEN.raw() + "\n";
      unmatchedFlag = true;
    }
  }
  if (unmatchedFlag) {
    llvm::report_fatal_error(everyUnmatched.c_str());
  }
}

std::string Lexer::readFile(const std::filesystem::path &fileName) const {
  std::fstream file{fileName};
  if (!file.is_open()) {
    const std::string strErr{"The file " + fileName.string() + " is not open"};
    llvm::report_fatal_error(strErr.c_str());
  }
  std::string text{""};
  std::string line{""};
  while (getline(file, line)) {
    text += line + "\n";
  }
  file.close();
  return text;
}

std::vector<Token> Lexer::analyze(const std::filesystem::path &fileName,
                                  bool verbose) const {
  const std::string TEXT{readFile(fileName)};
  const Category expression{concatCategories()};
  std::vector<Token> result{};
  std::string::const_iterator start, end, lastMatchEnd;
  start = TEXT.begin();
  end = TEXT.end();
  lastMatchEnd = start;
  std::match_results<std::string::const_iterator> what;
  std::size_t row{1};
  std::size_t col{1};
  while (std::regex_search(start, end, what, expression.pattern().pattern())) {
    if (lastMatchEnd != what[0].first) {
      const std::string UNMATCHED{lastMatchEnd, what[0].first};
      if (verbose) {
        std::cout << "Unmatched: " << UNMATCHED << "\n";
      }

      for (auto it = lastMatchEnd; it != what[0].first; ++it) {
        if (*it == '\n') {
          ++row;
          col = 1;
        } else {
          ++col;
        }
      }

      result.push_back(
          Token{TokenType::UNMATCHED, UNMATCHED, Location{fileName, row, col}});
    }
    for (const auto &category : categories_) {
      if (std::regex_match(what[0].str(), category.pattern().pattern())) {
        if (verbose) {
          std::cout << "Category: " << tokenTypeToString(category.type())
                    << ", Match: " << what[0].str() << " " << row << ":" << col
                    << "\n";
        }
        if (!category.skip()) {
          result.push_back(Token{category.type(), what[0].str(),
                                 Location{fileName, row, col}});
        }
        break;
      }
    }

    for (auto it = what[0].first; it != what[0].second; ++it) {
      if (*it == '\n') {
        ++row;
        col = 1;
      } else {
        ++col;
      }
    }

    lastMatchEnd = what[0].second; // Update last match end position
    start = what[0].second;        // update search position
  }
  // Check for unmatched tokens after the last match
  if (lastMatchEnd != end) {
    const std::string UNMATCHED{lastMatchEnd, end};
    if (verbose) {
      std::cout << "Unmatched: " << UNMATCHED << "\n";
    }

    for (auto it = lastMatchEnd; it != end; ++it) {
      if (*it == '\n') {
        ++row;
        col = 1;
      } else {
        ++col;
      }
    }

    result.push_back(
        Token{TokenType::UNMATCHED, UNMATCHED, Location{fileName, row, col}});
  }
  checkUnmatched(result);
  return result;
}

} // namespace nicole
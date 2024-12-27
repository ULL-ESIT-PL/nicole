#ifndef NICOLE_ERRORS_H
#define NICOLE_ERRORS_H

#include <llvm/IR/Attributes.h>
#include <ostream>

namespace nicole {

enum class ERROR_TYPE {
  // Options
  MISSING_ARGUMENTS,
  MISSING_BINARY_NAME,
  MISSING_ENTRY_FILE,
  ENTRY_FILE_NOT_FOUND,
  UNKOWN_ARGUMENT,

  // Lexer
  CHECK_UNMATCHED,
  FILE_EXTENSION,
  FILE_NOT_OPEN,

  // TokeStream
  EAT,
  CURRENT,
  LOOK_AHEAD,
  IS_CURRENT_TOKEN_TYPE,
  INSERT_AFTER

};

[[nodiscard]] inline std::string errorToStr(const ERROR_TYPE type) {
  switch (type) {
  // Options
  case ERROR_TYPE::MISSING_ARGUMENTS:
    return "MISSING_ARGUMENTS";
  case ERROR_TYPE::MISSING_BINARY_NAME:
    return "MISSING_BINARY_NAME";
  case ERROR_TYPE::MISSING_ENTRY_FILE:
    return "MISSING_ENTRY_FILE";
  case ERROR_TYPE::ENTRY_FILE_NOT_FOUND:
    return "ENTRY_FILE_NOT_FOUND";
  case ERROR_TYPE::UNKOWN_ARGUMENT:
    return "UNKOWN_ARGUMENT";

  // Lexer
  case ERROR_TYPE::CHECK_UNMATCHED:
    return "CHECK_UNMATCHED";
  case ERROR_TYPE::FILE_EXTENSION:
    return "FILE_EXTENSION";
  case ERROR_TYPE::FILE_NOT_OPEN:
    return "FILE_NOT_OPEN";

  // TokeStream
  case ERROR_TYPE::EAT:
    return "EAT";
  case ERROR_TYPE::CURRENT:
    return "CURRENT";
  case ERROR_TYPE::LOOK_AHEAD:
    return "LOOK_AHEAD";
  case ERROR_TYPE::IS_CURRENT_TOKEN_TYPE:
    return "IS_CURRENT_TOKEN_TYPE";
  case ERROR_TYPE::INSERT_AFTER:
    return "INSERT_AFTER";
  }
}

class Error final {
private:
  ERROR_TYPE type_;
  std::string info_{""};

public:
  explicit Error(const ERROR_TYPE type, const std::string &info) noexcept
      : type_{type}, info_{info} {}

  [[nodiscard]] ERROR_TYPE type() const { return type_; }

  [[nodiscard]] std::string info() const { return info_; }

  friend std::ostream &operator<<(std::ostream &os, const Error &error) {
    return os << "Error " << errorToStr(error.type_) << ": " << error.info_;
  }
};

} // namespace nicole

#endif
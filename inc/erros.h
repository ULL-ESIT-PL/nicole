#ifndef NICOLE_ERRORS_H
#define NICOLE_ERRORS_H

#include <llvm/IR/Attributes.h>
#include <ostream>
namespace nicole {

enum class ERROR_TYPE {

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

  friend std::ostream &operator<<(std::ostream &os, const Error &error) {
    return os << "Error " << errorToStr(error.type_) << ": " << error.info_;
  }
};

} // namespace nicole

#endif
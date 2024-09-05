#include "../../inc/lexer/tokenType.h"
#include <stdexcept>

namespace nicole {
std::string tokenTypeToString(const TokenType& type) {
  switch (type) {
    case TokenType::UNDEFINED:
      return "Undefined";
    case TokenType::NUMBER:
      return "Undefined";
    case TokenType::STRING:
      return "Undefined";
    case TokenType::SEMICOLON:
      return "Undefined";
    case TokenType::UNKNOWN:
      return "Unkown";
  }
}
};  // namespace nicole
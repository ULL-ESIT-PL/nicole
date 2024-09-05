#ifndef TOKEN_TYPE_H
#define TOKEN_TYPE_H

#include <exception>
#include <stdexcept>
#include <string>
using namespace std;

namespace nicole {

// ALREADY SORTED BASED ON MATCHING PREFERENCE
enum class TokenType { UNDEFINED, NUMBER, STRING, SEMICOLON, UNKNOWN };

std::string tokenTypeToString(const TokenType& type);

}  // namespace nicole

#endif

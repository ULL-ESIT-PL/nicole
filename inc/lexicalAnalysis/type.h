#ifndef TYPE_H
#define TYPE_H

#include <string>

namespace nicole {

// We use a enum so the parsing doesnt relly into knowing the raw data or a
// string comparasion between types which is slower
enum class TokenType {
  SPACE,
  COMMENT,
  ENDLINE,
  NUMBER_FLOAT,
  NUMBER_DOUBLE,
  NUMBER_INT,
  CHAR,
  STRING,
  TRUE,
  FALSE,
  LET,
  CONST,
  AUTO,
  IMPORT,
  PRINT,
  SYSTEM,
  TYPE,
  IF,
  ELSE,
  SWITCH,
  CASE,
  DEFAULT,
  WHILE,
  FOR,
  DO,
  STOP,
  PASS,
  RETURN,
  FUNCTION,
  STRUCT,
  OR,
  AND,
  ID,
  RP,
  LP,
  RB,
  LB,
  RC,
  LC,
  SEMICOLON,
  COMMA,
  TERNARY,
  DOTDOT,
  SMALLEREQUAL,
  BIGGEREQUAL,
  NOTEQUAL,
  EQUAL,
  ASSIGNMENT,
  INCREMENT,
  DECREMENT,
  OPERATOR_ADD,
  OPERATOR_SUB,
  OPERATOR_MULT,
  OPERATOR_DIV,
  OPERATOR_SMALLER,
  OPERATOR_GREATER,
  OPERATOR_NOT,
  OPERATOR_MODULE,
  UNMATCHED, // Para tokens no reconocidos
  ALL
};

std::string tokenTypeToString(const TokenType &tk);
} // namespace nicole

#endif
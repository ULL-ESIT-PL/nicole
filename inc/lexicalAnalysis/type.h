#ifndef TYPE_H
#define TYPE_H

namespace nicole {

enum class TokenType {
  SPACE,
  COMMENT,
  NUMBER_DOUBLE,
  NUMBER_INT,
  CHAR,
  STRING,
  TRUE,
  FALSE,
  CONST,
  IMPORT,
  PRINT,
  SYSTEM,
  TYPE,
  IF,
  ELSE,
  WHILE,
  FOR,
  STOP,
  PASS,
  RETURN,
  FUNCTION,
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
  OPERATOR,
  UNMATCHED,  // Para tokens no reconocidos
  ALL
};
}  // namespace nicole

#endif
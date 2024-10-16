#include "../../inc/lexicalAnalysis/type.h"

namespace nicole {

std::string tokenTypeToString(const TokenType &tk) {
  switch (tk) {
  case TokenType::SPACE:
    return "SPACE";
  case TokenType::COMMENT:
    return "COMMENT";
  case TokenType::ENDLINE:
    return "ENDLINE";
  case TokenType::NUMBER_FLOAT:
    return "NUMBER_FLOAT";
  case TokenType::NUMBER_DOUBLE:
    return "NUMBER_DOUBLE";
  case TokenType::NUMBER_INT:
    return "NUMBER_INT";
  case TokenType::CHAR:
    return "CHAR";
  case TokenType::STRING:
    return "STRING";
  case TokenType::TRUE:
    return "TRUE";
  case TokenType::FALSE:
    return "FALSE";
  case TokenType::LET:
    return "LET";
  case TokenType::CONST:
    return "CONST";
  case TokenType::IMPORT:
    return "IMPORT";
  case TokenType::PRINT:
    return "PRINT";
  case TokenType::SYSTEM:
    return "SYSTEM";
  case TokenType::TYPE:
    return "TYPE";
  case TokenType::IF:
    return "IF";
  case TokenType::ELSE:
    return "ELSE";
  case TokenType::SWITCH:
    return "SWITCH";
  case TokenType::CASE:
    return "CASE";
  case TokenType::WHILE:
    return "WHILE";
  case TokenType::FOR:
    return "FOR";
  case TokenType::DO:
    return "DO";
  case TokenType::STOP:
    return "STOP";
  case TokenType::PASS:
    return "PASS";
  case TokenType::RETURN:
    return "RETURN";
  case TokenType::FUNCTION:
    return "FUNCTION";
  case TokenType::STRUCT:
    return "STRUCT";
  case TokenType::OR:
    return "OR";
  case TokenType::AND:
    return "AND";
  case TokenType::ID:
    return "ID";
  case TokenType::RP:
    return "RP"; // Right Parenthesis ')'
  case TokenType::LP:
    return "LP"; // Left Parenthesis '('
  case TokenType::RB:
    return "RB"; // Right Bracket ']'
  case TokenType::LB:
    return "LB"; // Left Bracket '['
  case TokenType::RC:
    return "RC"; // Right Curly Brace '}'
  case TokenType::LC:
    return "LC"; // Left Curly Brace '{'
  case TokenType::SEMICOLON:
    return "SEMICOLON";
  case TokenType::COMMA:
    return "COMMA";
  case TokenType::TERNARY:
    return "TERNARY";
  case TokenType::DOTDOT:
    return "DOTDOT"; // :
  case TokenType::SMALLEREQUAL:
    return "SMALLEREQUAL";
  case TokenType::BIGGEREQUAL:
    return "BIGGEREQUAL";
  case TokenType::NOTEQUAL:
    return "NOTEQUAL";
  case TokenType::EQUAL:
    return "EQUAL";
  case TokenType::ASSIGNMENT:
    return "ASSIGNMENT"; // '='
  case TokenType::INCREMENT:
    return "INCREMENT"; // '++'
  case TokenType::DECREMENT:
    return "DECREMENT"; // '--'
  case TokenType::OPERATOR_ADD:
    return "OPERATOR_ADD"; // '+'
  case TokenType::OPERATOR_SUB:
    return "OPERATOR_SUB"; // '-'
  case TokenType::OPERATOR_MULT:
    return "OPERATOR_MULT"; // '*'
  case TokenType::OPERATOR_DIV:
    return "OPERATOR_DIV"; // '/'
  case TokenType::OPERATOR_SMALLER:
    return "OPERATOR_SMALLER"; // '<'
  case TokenType::OPERATOR_GREATER:
    return "OPERATOR_GREATER"; // '>'
  case TokenType::OPERATOR_NOT:
    return "OPERATOR_NOT"; // '!'
  case TokenType::OPERATOR_MODULE:
    return "OPERATOR_MODULE"; // '%'
  case TokenType::UNMATCHED:
    return "UNMATCHED"; // For unrecognized tokens
  case TokenType::ALL:
    return "ALL"; // Catch-all case for all tokens
  default:
    return "UNKNOWN"; // In case of an unrecognized token type
  }
}

} // namespace nicole

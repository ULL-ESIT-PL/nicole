#include "../../inc/lexicalAnalysis/nicoleSintax.h"

namespace nicole {

Lexer NicoleSintax::createLexer() const {
  return Lexer{std::vector<Category>{
      Category{TokenType::SPACE, "\\s+", true},
      Category{TokenType::COMMENT, "\\/\\*(.|\\n)*?\\*\\/", true},

      Category{TokenType::NUMBER_FLOAT, "f\\d+\\.\\d+([eE][-]?\\d+)?", false},
      Category{TokenType::NUMBER_DOUBLE, "\\d+\\.\\d+([eE][-]?\\d+)?", false},
      Category{TokenType::NUMBER_INT, "\\d+([eE][-]?\\d+)?", false},
      Category{TokenType::CHAR, "\'(?:\\\\.|[^\'\\\\])\'", false},
      Category{TokenType::STRING, "\"(?:\\\\.|[^\"\\\\])*\"", false},
      Category{TokenType::TRUE, "true", false},
      Category{TokenType::FALSE, "false", false},

      Category{TokenType::CONST, "const", false},
      Category{TokenType::LET, "let", false},
      Category{TokenType::AUTO, "auto", false},

      Category{TokenType::IMPORT, "import", false},

      Category{TokenType::PRINT, "print", false},
      Category{TokenType::SYSTEM, "system", false},
      Category{TokenType::TYPE, "type", false},

      Category{TokenType::SWITCH, "switch", false},
      Category{TokenType::CASE, "case", false},
      Category{TokenType::DEFAULT, "default", false},

      Category{TokenType::IF, "if", false},
      Category{TokenType::ELSE, "else", false},
      Category{TokenType::WHILE, "while", false},
      Category{TokenType::FOR, "for", false},
      Category{TokenType::STOP, "stop", false},
      Category{TokenType::PASS, "pass", false},
      Category{TokenType::RETURN, "return", false},
      Category{TokenType::FUNCTION, "def", false},
      Category{TokenType::STRUCT, "struct", false},

      Category{TokenType::OR, "or", false},
      Category{TokenType::AND, "and", false},

      // avoids the conflic between do and double
      Category{TokenType::ID, "(?!\\b(do)\\b)[a-zA-Z]+[a-zA-Z0-9]*", false},

      Category{TokenType::DO, "do", false},

      Category{TokenType::RP, "\\)", false},
      Category{TokenType::LP, "\\(", false},
      Category{TokenType::RB, "\\}", false},
      Category{TokenType::LB, "\\{", false},
      Category{TokenType::RC, "\\]", false},
      Category{TokenType::LC, "\\[", false},
      Category{TokenType::SEMICOLON, ";", false},
      Category{TokenType::COMMA, ",", false},
      Category{TokenType::TERNARY, "\\?", false},
      Category{TokenType::DOTDOT, ":", false},

      Category{TokenType::SMALLEREQUAL, "<=", false},
      Category{TokenType::BIGGEREQUAL, ">=", false},
      Category{TokenType::NOTEQUAL, "!=", false},
      Category{TokenType::EQUAL, "==", false},

      Category{TokenType::ASSIGNMENT, "=", false},

      Category{TokenType::INCREMENT, "\\+\\+", false},
      Category{TokenType::DECREMENT, "\\-\\-", false},
      Category{TokenType::OPERATOR_ADD, "[+]", false},
      Category{TokenType::OPERATOR_SUB, "[-]", false},
      Category{TokenType::OPERATOR_MULT, "[\\*]", false},
      Category{TokenType::OPERATOR_DIV, "[/]", false},
      Category{TokenType::OPERATOR_SMALLER, "[<]", false},
      Category{TokenType::OPERATOR_GREATER, "[>]", false},
      Category{TokenType::OPERATOR_NOT, "[!]", false},
      Category{TokenType::OPERATOR_MODULE, "[%]", false},
  }};
};

} // namespace nicole
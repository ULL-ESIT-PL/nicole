#ifndef NICOLE_SINTAX_H
#define NICOLE_SINTAX_H

#include "lexer.h"

namespace nicole {

class NicoleSintax {
 private:
    NicoleSintax() = default;
 public:
  static Lexer createLexer() {
    return Lexer{std::vector<Category>{
        Category{TokenType::SPACE, "\\s+", true},
        Category{TokenType::COMMENT, "\\/\\*(.|\\n)*?\\*\\/", true},

        Category{TokenType::NUMBER, "\\d+(\\.\\d+)?([eE][-]?\\d+)?", false},
        Category{TokenType::STRING, "\"(?:\\\\.|[^\"\\\\])*\"", false},
        Category{TokenType::TRUE, "true", false},
        Category{TokenType::FALSE, "false", false},

        Category{TokenType::CONST, "const", false},

        Category{TokenType::IMPORT, "import", false},

        Category{TokenType::PRINT, "print", false},
        Category{TokenType::SYSTEM, "system", false},
        Category{TokenType::TYPE, "type", false},

        Category{TokenType::IF, "if", false},
        Category{TokenType::ELSE, "else", false},
        Category{TokenType::WHILE, "while", false},
        Category{TokenType::FOR, "for", false},
        Category{TokenType::STOP, "stop", false},
        Category{TokenType::PASS, "pass", false},
        Category{TokenType::RETURN, "return", false},
        Category{TokenType::FUNCTION, "def", false},

        Category{TokenType::OR, "or", false},
        Category{TokenType::AND, "and", false},

        Category{TokenType::ID, "[a-zA-Z]+[a-zA-Z0-9]*", false},

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

        Category{TokenType::INCREMENT, "\\+\\+|\\-\\-", false},
        Category{TokenType::OPERATOR, "[+-/\\*<>!%]", false},
    }};
  };
};

}  // namespace nicole

#endif
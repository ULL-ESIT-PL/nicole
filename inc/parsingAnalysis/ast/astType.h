#ifndef AST_TYPE_H
#define AST_TYPE_H

#include <string>

namespace nicole {

enum class AST_TYPE: int {
// Literals
BOOL,
INT,
CHAR,
DOUBLE,
FLOAT,
STRING,
NULLPTR,

// Vectors
VECTOR,

// Pointers
PTR,
PTR_DEREFENCE,

// Binary operators
ADD,
SUB,
MULT,
DIV,
MODULE,
SMALLER,
SMALLER_EQUAL,
BIGGER,
BIGGER_EQUAL,
EQUAL,
NOT_EQUAL,
OR,
AND,

// Unary operators
NOT,
INCREMENT,
DECREMENT,

// Variable declaration
LET_DECL,
CONST_DECL,
AUTO_DECL,
VAR_CALL,

// Asignment
ASIGNMENT,
SELF_ADD,
SELF_SUB,
SELF_MULT,
SELF_DIV,

// Utils
PRINT,
IMPORT,

// Statements
STATEMENT,
BODY,
COMMA,

// Loops
WHILE,
FOR,
DO_WHILE,
PASS,
STOP,

// Conditionals
IF,
SWITCH,
CASE,
TERNARY,

// Functions
FUN_DECL,
RETURN,
FUN_CALL,

// Structs
STRUCT_DECL,

// Classes
CLASS_DECL,

// Chained expression
CHAIN

};

[[nodiscard]] std::string astTypeToStr(const AST_TYPE type) noexcept;

}

#endif
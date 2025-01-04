#include "../../../inc/parsingAnalysis/ast/astType.h"

namespace nicole {

std::string astTypeToStr(const AST_TYPE type) noexcept {
  switch (type) {
    // Literals
  case AST_TYPE::BOOL:
    return "BOOL";
  case AST_TYPE::INT:
    return "INT";
  case AST_TYPE::CHAR:
    return "CHAR";
  case AST_TYPE::DOUBLE:
    return "DOUBLE";
  case AST_TYPE::FLOAT:
    return "FLOAT";
  case AST_TYPE::STRING:
    return "STRING";
  case AST_TYPE::NULLPTR:
    return "NULLPTR";

    // Vectors
  case AST_TYPE::VECTOR:
    return "VECTOR";

    // Pointers
  case AST_TYPE::PTR:
    return "PTR";
  case AST_TYPE::PTR_DEREFENCE:
    return "PTR_DEREFENCE";
  case AST_TYPE::NEW:
    return "NEW";
  case AST_TYPE::DELETE:
    return "DELETE";

    // Binary operators
  case AST_TYPE::ADD:
    return "ADD";
  case AST_TYPE::SUB:
    return "SUB";
  case AST_TYPE::MULT:
    return "MULT";
  case AST_TYPE::DIV:
    return "DIV";
  case AST_TYPE::MODULE:
    return "MODULE";
  case AST_TYPE::SMALLER:
    return "SMALLER";
  case AST_TYPE::SMALLER_EQUAL:
    return "SMALLER_EQUAL";
  case AST_TYPE::BIGGER:
    return "BIGGER";
  case AST_TYPE::BIGGER_EQUAL:
    return "BIGGER_EQUAL";
  case AST_TYPE::EQUAL:
    return "EQUAL";
  case AST_TYPE::NOT_EQUAL:
    return "NOT_EQUAL";
  case AST_TYPE::OR:
    return "OR";
  case AST_TYPE::AND:
    return "AND";

  // Unary operators
  case AST_TYPE::NEG:
    return "NEG";
  case AST_TYPE::NOT:
    return "NOT";
  case AST_TYPE::INCREMENT:
    return "INCREMENT";
  case AST_TYPE::DECREMENT:
    return "DECREMENT";

    // Variable declaration
  case AST_TYPE::LET_DECL:
    return "LET_DECL";
  case AST_TYPE::CONST_DECL:
    return "CONST_DECL";
  case AST_TYPE::AUTO_DECL:
    return "AUTO_DECL";
  case AST_TYPE::VAR_CALL:
    return "VAR_CALL";

    // Asignment
  case AST_TYPE::ASIGNMENT:
    return "ASIGNMENT";
  case AST_TYPE::SELF_ADD:
    return "SELF_ADD";
  case AST_TYPE::SELF_SUB:
    return "SELF_SUB";
  case AST_TYPE::SELF_MULT:
    return "SELF_MULT";
  case AST_TYPE::SELF_DIV:
    return "SELF_DIV";

    // Utils
  case AST_TYPE::PRINT:
    return "PRINT";
  case AST_TYPE::IMPORT:
    return "IMPORT";

    // Statements
  case AST_TYPE::STATEMENT:
    return "STATEMENT";
  case AST_TYPE::BODY:
    return "BODY";
  case AST_TYPE::COMMA:
    return "COMMA";

    // Loops
  case AST_TYPE::WHILE:
    return "WHILE";
  case AST_TYPE::FOR:
    return "FOR";
  case AST_TYPE::DO_WHILE:
    return "DO_WHILE";
  case AST_TYPE::PASS:
    return "PASS";
  case AST_TYPE::STOP:
    return "STOP";

    // Conditionals
  case AST_TYPE::IF:
    return "IF";
  case AST_TYPE::SWITCH:
    return "SWITCH";
  case AST_TYPE::CASE:
    return "CASE";
  case AST_TYPE::TERNARY:
    return "TERNARY";

    // Functions
  case AST_TYPE::FUNC_DECL:
    return "FUNC_DECL";
  case AST_TYPE::RETURN:
    return "RETURN";
  case AST_TYPE::FUN_CALL:
    return "FUN_CALL";

    // Structs
  case AST_TYPE::STRUCT_DECL:
    return "STRUCT_DECL";

    // Classes
  case AST_TYPE::CLASS_DECL:
    return "CLASS_DECL";

    // Enum
  case AST_TYPE::ENUM:
    return "ENUM";

  // Chained expression
  case AST_TYPE::CHAIN:
    return "CHAIN";
  }
}

} // namespace nicole
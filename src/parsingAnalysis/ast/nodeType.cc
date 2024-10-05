#include "../../../inc/parsingAnalysis/ast/nodeType.h"

namespace nicole {
std::string nodeTypeToString(const NodeType &nt) {
  switch (nt) {
  case NodeType::VOID:
    return "VOID";
  case NodeType::CHAR:
    return "CHAR";
  case NodeType::STRING:
    return "STRING";
  case NodeType::INT:
    return "INT";
  case NodeType::FLOAT:
    return "FLOAT";
  case NodeType::DOUBLE:
    return "DOUBLE";
  case NodeType::BOOL:
    return "BOOL";
  case NodeType::BINARY:
    return "BINARY";
  case NodeType::UNARY:
    return "UNARY";
  case NodeType::INCREMENT:
    return "INCREMENT";
  case NodeType::STATEMENT:
    return "STATEMENT";
  case NodeType::STATEMENT_LIST:
    return "STATEMENT_LIST";
  case NodeType::VAR_DECL:
    return "VAR_DECL";
  case NodeType::CONST_DECL:
    return "CONST_DECL";
  case NodeType::STRUCT_DECL:
    return "STRUCT_DECL";
  case NodeType::VAR_REG:
    return "VAR_REG";
  case NodeType::CALL_VAR:
    return "CALL_VAR";
  case NodeType::CALL_FUNCTION:
    return "CALL_FUNCTION";
  case NodeType::IF:
    return "IF";
  case NodeType::WHILE:
    return "WHILE";
  case NodeType::FOR:
    return "FOR";
  case NodeType::DO_WHILE:
    return "DO_WHILE";
  case NodeType::STOP:
    return "STOP";
  case NodeType::PASS:
    return "PASS";
  case NodeType::PRINT:
    return "PRINT";
  case NodeType::CALL_CTR:
    return "CALL_CTR";
  case NodeType::FUN_DECL:
    return "FUN_DECL";
  case NodeType::RETURN:
    return "RETURN";
  }
}
} // namespace nicole

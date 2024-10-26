#ifndef NODE_TYPE_H
#define NODE_TYPE_H

#include <string>

namespace nicole {

// To know with what kind of node are we working
enum class NodeType {
  LITERAL,
  VOID,
  CHAR,
  STRING,
  INT,
  FLOAT,
  DOUBLE,
  BOOL,
  IMPORT,
  BINARY,
  UNARY,
  INCREMENT,
  STATEMENT,
  STATEMENT_LIST,
  CONST_DECL,
  VAR_DECL,
  AUTO_DECL,
  STRUCT_DECL,
  FUN_DECL,
  STRUCT_SET_ATTR,
  VAR_REG,
  SELF_REG,
  STRUCT_ACS,
  CALL_VAR,
  CALL_FUNCTION,
  CALL_CTR,
  IF,
  SWITCH,
  CASE,
  WHILE,
  FOR,
  DO_WHILE,
  STOP,
  PASS,
  RETURN,
  PRINT
};

std::string nodeTypeToString(const NodeType& nt);

}  // namespace nicole

#endif
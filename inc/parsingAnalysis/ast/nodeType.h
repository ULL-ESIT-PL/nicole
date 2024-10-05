#ifndef NODE_TYPE_H
#define NODE_TYPE_H

#include <string>

namespace nicole {

enum class NodeType {
  VOID,
  CHAR,
  STRING,
  INT,
  FLOAT,
  DOUBLE,
  BOOL,
  BINARY,
  UNARY,
  INCREMENT,
  STATEMENT,
  STATEMENT_LIST,
  CONST_DECL,
  VAR_DECL,
  STRUCT_DECL,
  FUN_DECL,
  VAR_REG,
  CALL_VAR,
  CALL_FUNCTION,
  CALL_CTR,
  IF,
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
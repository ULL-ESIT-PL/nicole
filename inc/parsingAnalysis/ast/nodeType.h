#ifndef NODE_TYPE_H
#define NODE_TYPE_H

#include <string>

namespace nicole {

enum class NodeType {
  LITERAL,
  CHAR,
  STRING,
  INT,
  DOUBLE,
  BOOL,
  BINARY,
  STATEMENT,
  STATEMENT_LIST,
  CONST_DECL,
  VAR_DECL,
  VAR_REG,
  CALL_VAR,
  IF
};

std::string nodeTypeToString(const NodeType& nt);

}  // namespace nicole

#endif
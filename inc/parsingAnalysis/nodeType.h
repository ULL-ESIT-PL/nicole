#ifndef NODE_TYPE_H
#define NODE_TYPE_H

namespace nicole {

enum class NodeType {
  CHAR,
  STRING,
  INT,
  DOUBLE,
  BOOL,
  BINARY,
  STATEMENT,
  STATEMENT_LIST,
  VAR_DECL
};

}  // namespace nicole

#endif
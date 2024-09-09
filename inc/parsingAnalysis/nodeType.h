#ifndef NODE_TYPE_H
#define NODE_TYPE_H

namespace nicole {
enum class NodeType { CHAR, STRING, INT, DOUBLE, BOOL, BINARY, STATEMENT };

enum class Operator {
  ADD,
  SUB,
  MULT,
  DIV,
  MODULE,
};
}  // namespace nicole

#endif
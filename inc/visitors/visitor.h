#ifndef VISITOR_H
#define VISITOR_H

#include "../parsingAnalysis/literals/nodeLiteralInt.h"
#include "../parsingAnalysis/operations/nodeBinary.h"
#include "../parsingAnalysis/statements/statement.h"

namespace nicole {
class Visitor {
 public:
  virtual ~Visitor() = default;

  // Métodos específicos para los diferentes tipos de nodos
  virtual llvm::Value* visit(const std::unique_ptr<NodeLiteralInt>& node) const = 0;

  virtual llvm::Value* visit(const std::unique_ptr<NodeBinary>& node) const = 0;

  virtual llvm::Value* visit(const std::unique_ptr<NodeStatement>& node) const = 0;
};

}  // namespace nicole

#endif
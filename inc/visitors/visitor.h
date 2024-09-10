#ifndef VISITOR_H
#define VISITOR_H

#include "../parsingAnalysis/literals/nodeLiteralInt.h"
#include "../parsingAnalysis/operations/nodeBinary.h"

namespace nicole {
class Visitor {
 public:
  virtual ~Visitor() = default;
  
  virtual llvm::Value* visit(const std::unique_ptr<NodeLiteralInt>& node) = 0;

  virtual llvm::Value* visit(const std::unique_ptr<NodeBinary>& node) = 0;
};

}  // namespace nicole

#endif
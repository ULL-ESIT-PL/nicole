#include "../../inc/visitors/codeGeneration.h"

namespace nicole
{
  llvm::Value* CodeGeneration::visit(const std::unique_ptr<NodeLiteralInt>& node) const {

  }

  llvm::Value* CodeGeneration::visit(const std::unique_ptr<NodeBinary>& node) const {}

  llvm::Value* CodeGeneration::visit(const std::unique_ptr<NodeStament>& node) const {}
} // namespace nicole

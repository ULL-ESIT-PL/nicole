#include "../../../inc/visitors/codeGeneration.h"

#include "../../../inc/parsingAnalysis/ast/calls/functionCall.h"
#include "../../../inc/parsingAnalysis/ast/calls/structConstructor.h"
#include "../../../inc/parsingAnalysis/ast/calls/variableCall.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/ErrorHandling.h"
#include <ostream>
#include <string>
#include <vector>

namespace nicole {

llvm::Value *CodeGeneration::visit(const NodeStructConstructor *node) const {
  llvm::report_fatal_error("hola");
  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeVariableCall *node) const {
  std::cout << "---------\n" << *node->table() << std::flush;
  return builder_.CreateLoad(
      node->table()->variableValue(node->id())->getType(),
      node->table()->variableAddress(node->id()), node->id());
}

llvm::Value *CodeGeneration::visit(const NodeFunctionCall *node) const {
  std::cout << "---------\n" << *node->functionTable() << std::flush;
  auto func{node->functionTable()->function(node->id())};
  std::vector<llvm::Value *> args{};
  for (const auto &param : *node) {
    args.push_back(param->accept(this));
  }
  if (func->arg_size() != args.size()) {
    const std::string errStr{
        "Invalid arguments size for function: " + node->id() +
        ", the size is " + std::to_string(func->arg_size())};
    llvm::report_fatal_error(errStr.c_str());
  }
  return builder_.CreateCall(func, args, "call_" + node->id());
}

} // namespace nicole
#include "../../../inc/visitors/codeGeneration.h"

#include "../../../inc/parsingAnalysis/ast/utils/nodeImport.h"
#include "../../../inc/parsingAnalysis/ast/utils/nodePrint.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/ErrorHandling.h"
#include <string>
#include <vector>

namespace nicole {

llvm::Value *CodeGeneration::visit(const NodePrint *node) const {
  std::vector<llvm::Value *> values{};
  std::string fullFormatStr = "";

  for (const auto &expr : node->expressions()) {
    auto value = expr->accept(this);
    if (!value) {
      llvm::report_fatal_error("Expression evaluation failed.");
    }
    values.push_back(value);
  }

  auto paramsAndFormats{printParameters(values, context_, builder_)};
  std::vector<llvm::Value *> args{};

  for (const auto &paramAndFormat : paramsAndFormats) {
    fullFormatStr += paramAndFormat.second; // Concatenar el formato
    args.push_back(paramAndFormat.first);   // Añadir el valor
  }

  // Check if printf already exists in the module
  llvm::Function *printfFunc = module_->getFunction("printf");
  if (!printfFunc) {
    llvm::FunctionType *printfType = llvm::FunctionType::get(
        llvm::IntegerType::getInt32Ty(*context_),
        llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(*context_)), true);
    printfFunc = llvm::Function::Create(
        printfType, llvm::Function::ExternalLinkage, "printf", module_);
  }

  // Crear una cadena de formato y pasarla junto con los valores a printf
  llvm::Value *formatStr = builder_.CreateGlobalStringPtr(fullFormatStr, "fmt");
  args.insert(args.begin(),
              formatStr); // Insert the format at the beginning of the arguments

  // call printf
  builder_.CreateCall(printfFunc, args, "calltmp");

  return nullptr;
}

llvm::Value *CodeGeneration::visit(const NodeImport *node) const {
  return nullptr;
}

} // namespace nicole
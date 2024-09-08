#include "../../../inc/parsingAnalysis/literals/nodeLiteralString.h"

#include <iostream>
namespace nicole {
llvm::Value* NodeLiteralString::codeGeneration() const {
  llvm::IRBuilder<> builder{*context_};
  llvm::Value* str{builder.CreateGlobalString(llvm::StringRef{value_}, "str", 0U, module_)};
  return builder.CreatePointerCast(str, builder.getInt8PtrTy());
}
}  // namespace nicole

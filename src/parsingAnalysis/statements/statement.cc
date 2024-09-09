#include "../../../inc/parsingAnalysis/statements/statement.h"
#include <iostream>
namespace nicole
{
 llvm::Value* NodeStatement::codeGeneration() const {
   return expression_->codeGeneration();
 }   
} // namespace nicole

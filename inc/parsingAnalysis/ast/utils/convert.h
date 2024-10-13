#ifndef CONVERT_TYPE_H
#define CONVERT_TYPE_H

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Value.h"
#include <utility>

namespace nicole {

std::vector<std::pair<llvm::Value *, std::string>>
printParameters(std::vector<llvm::Value*> values, llvm::LLVMContext *context,
                llvm::IRBuilder<> &builder);

}

#endif
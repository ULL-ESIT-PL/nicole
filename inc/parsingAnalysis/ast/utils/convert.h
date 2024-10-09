#ifndef CONVERT_TYPE_H
#define CONVERT_TYPE_H

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Value.h"
#include <utility>

namespace nicole {

std::pair<llvm::Value *, std::string>
printParameters(llvm::Value *value, llvm::LLVMContext *context,
                llvm::IRBuilder<> &builder);

}

#endif
#ifndef CONVERT_TYPE_H
#define CONVERT_TYPE_H

#include "llvm/IR/Value.h"

namespace nicole {

std::string llvmValueToString(llvm::Value* value);

}

#endif
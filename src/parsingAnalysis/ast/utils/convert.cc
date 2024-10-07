#include "../../../../inc/parsingAnalysis/ast/utils/convert.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/ErrorHandling.h"

namespace nicole {

/*
  Faltan estructuras, punteros, null
*/

// d int, f float, s string, cchar, p ptr

std::pair<llvm::Value *, std::string>
printParameters(llvm::Value *value, llvm::LLVMContext *context,
                llvm::IRBuilder<> &builder) {
  std::string strFormat{""};
  if (!value) {
    llvm::report_fatal_error("Failed to evaluate expression for print.");
  }

  // EN CASO DE QUE SEA UN LOAD
  if (auto loadInst = llvm::dyn_cast<llvm::LoadInst>(value)) {
    llvm::Type *loadedType = loadInst->getType();
    if (loadedType->isIntegerTy(8)) {
      strFormat = "%c";
    } else if (loadedType->isIntegerTy()) {
      strFormat = "%d";
    } else if (loadedType->isFloatTy()) {
      strFormat = "%f";
      llvm::Type *doubleType = llvm::Type::getDoubleTy(*context);
      value = builder.CreateFPExt(loadInst, doubleType, "floatToDouble");
    } else if (loadedType->isDoubleTy()) {
      strFormat = "%f";
    } else if (loadedType->isPointerTy() &&
               llvm::dyn_cast<llvm::PointerType>(loadedType)
                   ->isValidElementType(llvm::Type::getInt8Ty(*context))) {
      strFormat = "%s";
    }
  } /* Temporals*/
  else if (value->getType()->isIntegerTy(8)) {
    strFormat = "%c";
  } else if (value->getType()->isIntegerTy()) {
    strFormat = "%d";
  } else if (value->getType()->isFloatTy()) {
    strFormat = "%f";
    llvm::Type *doubleType = llvm::Type::getDoubleTy(*context);
    value = builder.CreateFPExt(value, doubleType, "floatToDouble");
  } else if (value->getType()->isDoubleTy()) {
    strFormat = "%f";
  } else if (value->getType()->isPointerTy() &&
             llvm::dyn_cast<llvm::PointerType>(value->getType())
                 ->isValidElementType(llvm::Type::getInt8Ty(*context))) {
    strFormat = "%s";
  }

  if (!strFormat.size()) {
    llvm::report_fatal_error("Cannot print this type");
  }

  return {value, strFormat};
}

} // namespace nicole
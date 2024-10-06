#include "../../../../inc/parsingAnalysis/ast/utils/convert.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/ErrorHandling.h"
#include <iostream>
namespace nicole {

/*
En load funciona bool, int, float, double, char, str (parece que funciona)

En temp

En literal
*/

// d int, f float, s string, cchar, p ptr

std::pair<llvm::Value*, std::string> printParameters(llvm::Value* value, llvm::LLVMContext* context, llvm::IRBuilder<>& builder) {
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
            llvm::dyn_cast<llvm::PointerType>(loadedType)->isValidElementType(llvm::Type::getInt8Ty(*context))) {
      strFormat = "%s";
    }
  }


  /*
  // LOAD
  if (auto loadInst = llvm::dyn_cast<llvm::LoadInst>(value)) {
    // Get the type of the loaded value
    llvm::Type *loadedType = loadInst->getType();

    // Handle integer types
    if (llvm::isa<llvm::IntegerType>(loadedType)) {
      if (loadedType->isIntegerTy(1)) { // boolean
        strFormat = "%s\n";          // Use %s for boolean representation
      //  value = builder_.CreateICmpNE(loadInst,
      //                                llvm::ConstantInt::get(loadedType, 0));
        value = llvm::ConstantDataArray::getString(*context, "true");
      } else {
        strFormat = "%d\n"; // Use %d for integers
      }
    } else if (loadedType->isFloatTy()) { // Check for float type
      strFormat = "%f\n";              // Use %f for float representation
      // You might want to handle how to print the float value here
    }
    // Handle floating point types

    // Handle string types
    else if (auto globalString = llvm::dyn_cast<llvm::GlobalVariable>(
                 loadInst->getPointerOperand())) {
      if (auto initializer = llvm::dyn_cast<llvm::ConstantDataArray>(
              globalString->getInitializer())) {
        if (initializer->isString()) {
          strFormat = "%s\n"; // Use %s for strings
          value = globalString;  // Use global string variable
        }
      }
    } else {
      llvm::report_fatal_error(
          "Unsupported type for print from variable call.");
    }
  } 
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  else if (auto constantInt = llvm::dyn_cast<llvm::ConstantInt>(value)) {
    if (constantInt->getType()->isIntegerTy(1)) { // boolean
      strFormat = "%s\n"; // Use %s for boolean representation
      value = constantInt->isZero()
                  ? llvm::ConstantDataArray::getString(*context, "false")
                  : llvm::ConstantDataArray::getString(*context, "true");
                 // llvm::report_fatal_error("hola Bool LIteral");
    } else {
      strFormat = "%d\n"; // Use %d for integers
      llvm::report_fatal_error("hola Int LIteral");
    }
  }
  // Handle floating point types
  else if (auto constantFP = llvm::dyn_cast<llvm::ConstantFP>(value)) {
    strFormat = "%f\n"; // Use %f for floating points
  }
  // Handle string types
  else if (auto globalString = llvm::dyn_cast<llvm::GlobalVariable>(value)) {
    if (auto initializer = llvm::dyn_cast<llvm::ConstantDataArray>(
            globalString->getInitializer())) {
      if (initializer->isString()) {
        strFormat = "%s\n"; // Use %s for strings
        value = globalString;  // Use global string variable
      }
    }
  }
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Para cuando value es un temporal
  else if (auto intValue =
               llvm::dyn_cast<llvm::IntegerType>(value->getType())) {
    // Handle integer types
    if (intValue->isIntegerTy(1)) { // boolean
      strFormat = "%s\n";
      value = constantInt->isZero()
                  ? llvm::ConstantDataArray::getString(*context, "false")
                  : llvm::ConstantDataArray::getString(*context, "true");
    } else {
      strFormat = "%d\n"; // Use %d for integers
    }
  } else {
    llvm::report_fatal_error("Unsupported type for print.");
  }
  */
  std::cout << strFormat << std::flush;
  return {value, strFormat};
}

}
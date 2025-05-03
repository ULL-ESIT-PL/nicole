#include "../../../inc/parsingAnalysis/ast/utils/ast_import.h"
#include "../../../inc/parsingAnalysis/ast/utils/ast_print.h"
#include "../../../inc/visitors/codeGeneration/codeGeneration.h"
#include <cstddef>
#include <memory>
#include <variant>

namespace nicole {

/*
std::vector<std::pair<llvm::Value *, std::string>>
printParameters(std::vector<llvm::Value *> values, llvm::LLVMContext &context,
                llvm::IRBuilder<> &builder) {
  std::vector<std::pair<llvm::Value *, std::string>> paramsAndFormats{};
  for (auto value : values) {
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
        llvm::Type *doubleType = llvm::Type::getDoubleTy(context);
        value = builder.CreateFPExt(loadInst, doubleType, "floatToDouble");
      } else if (loadedType->isDoubleTy()) {
        strFormat = "%f";
      } else if (loadedType->isPointerTy() &&
                 llvm::dyn_cast<llvm::PointerType>(loadedType)
                     ->isValidElementType(llvm::Type::getInt8Ty(context))) {
        strFormat = "%s";
      }
    } //  Temporals
    else if (value->getType()->isIntegerTy(8)) {
      strFormat = "%c";
    } else if (value->getType()->isIntegerTy()) {
      strFormat = "%d";
    } else if (value->getType()->isFloatTy()) {
      strFormat = "%f";
      llvm::Type *doubleType = llvm::Type::getDoubleTy(context);
      value = builder.CreateFPExt(value, doubleType, "floatToDouble");
    } else if (value->getType()->isDoubleTy()) {
      strFormat = "%f";
    } else if (value->getType()->isPointerTy() &&
               llvm::dyn_cast<llvm::PointerType>(value->getType())
                   ->isValidElementType(llvm::Type::getInt8Ty(context))) {
      strFormat = "%s";
    }

    if (!strFormat.size()) {
      llvm::report_fatal_error("Cannot print this type: ");
    }

    paramsAndFormats.push_back({value, strFormat});
  }

  return paramsAndFormats;
}
*/
std::vector<std::pair<llvm::Value *, std::string>>
printParameters(const std::vector<llvm::Value *> &values,
                llvm::LLVMContext &context, llvm::IRBuilder<> &builder) {
  std::vector<std::pair<llvm::Value *, std::string>> out;
  out.reserve(values.size());

  // Crear una instancia de i8* para comparación
  llvm::Type *i8Ty = llvm::Type::getInt8Ty(context);
  llvm::Type *i8PtrTy = i8Ty->getPointerTo(/*AddressSpace=*/0);

  for (auto *origVal : values) {
    if (!origVal)
      llvm::report_fatal_error("Failed to evaluate expression for print.");

    llvm::Value *val = origVal;
    llvm::Type *ty = val->getType();
    std::string fmt;

    if (ty->isIntegerTy(8)) {
      fmt = "%c";
    } else if (ty->isIntegerTy()) {
      fmt = "%d";
    } else if (ty->isFloatTy()) {
      fmt = "%f";
      val = builder.CreateFPExt(val, llvm::Type::getDoubleTy(context), "fpext");
    } else if (ty->isDoubleTy()) {
      fmt = "%f";
    } else if (ty->isPointerTy() && ty == i8PtrTy) {
      // Cadena C (i8*)
      fmt = "%s";
    }

    if (fmt.empty())
      llvm::report_fatal_error("Cannot print this type.");

    out.emplace_back(val, fmt);
  }

  return out;
}

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_PRINT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_PRINT");
  }
  /*
  std::vector<llvm::Value *> values{};
  std::string fullFormatStr = "";
  for (const auto &chain : node->values()) {
    const auto result{chain->accept(*this)};
    if (!result) {
      return createError(result.error());
    }
    values.push_back(*result);
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
        llvm::IntegerType::getInt32Ty(context_),
        llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context_)), true);
    printfFunc = llvm::Function::Create(
        printfType, llvm::Function::ExternalLinkage, "printf", module_.get());
  }

  // Crear una cadena de formato y pasarla junto con los valores a printf
  llvm::Value *formatStr = builder_.CreateGlobalStringPtr(fullFormatStr, "fmt");
  args.insert(args.begin(),
              formatStr); // Insert the format at the beginning of the arguments

  // call printf
  builder_.CreateCall(printfFunc, args, "calltmp");

  return nullptr;
  */
  std::vector<llvm::Value *> values;
  values.reserve(node->values().size());
  for (const auto &chain : node->values()) {
    auto result = chain->accept(*this);
    if (!result)
      return createError(result.error());
    values.push_back(*result);
  }

  // Llamar a printParameters (sin cambios respecto a tu código)
  auto paramsAndFormats = printParameters(values, context_, builder_);

  // Concatenar el formato completo y preparar los argumentos
  std::string fullFormatStr;
  fullFormatStr.reserve(paramsAndFormats.size() * 2);
  std::vector<llvm::Value *> args;
  args.reserve(paramsAndFormats.size() + 1);
  for (auto &pf : paramsAndFormats) {
    fullFormatStr += pf.second; // añade "%d", "%f", "%s", …
    args.push_back(pf.first); // añade el llvm::Value* (posiblemente extendido)
  }

  // Obtener/declarar printf con getOrInsertFunction
  llvm::Type *i8Ty = llvm::Type::getInt8Ty(context_);
  llvm::Type *i8PtrTy = i8Ty->getPointerTo(0);
  auto printfTy = llvm::FunctionType::get(llvm::Type::getInt32Ty(context_),
                                          {i8PtrTy}, /*isVarArg=*/true);
  auto printfFn = module_->getOrInsertFunction("printf", printfTy);

  // Crear la cadena de formato global y añadirla como primer argumento
  llvm::Value *fmtStr = builder_.CreateGlobalStringPtr(fullFormatStr, "fmt");
  args.insert(args.begin(), fmtStr);

  // Emitir la llamada a printf y devolverla
  llvm::CallInst *call = builder_.CreateCall(printfFn, args, "calltmp");
  return call;
}

std::expected<llvm::Value *, Error>
CodeGeneration::visit(const AST_IMPORT *node) const noexcept {
  if (!node) {
    return createError(ERROR_TYPE::NULL_NODE, "invalid AST_IMPORT");
  }
  return {};
}

} // namespace nicole
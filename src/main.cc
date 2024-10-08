#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>

#include "../inc/parsingAnalysis/parsingAlgorithms/topDown.h"
#include "../inc/visitors/codeGeneration.h"
#include "../inc/visitors/printTree.h"
#include "llvm/Support/ErrorHandling.h"

using namespace nicole;

int main(int argc, char *argv[]) {
  // Start LLVM
  llvm::LLVMContext context;
  llvm::IRBuilder<> builder{context};
  std::unique_ptr<llvm::Module> module{
      std::make_unique<llvm::Module>("my_module", context)};

  // Crear una función main y un bloque básico
  llvm::FunctionType *funcType{
      llvm::FunctionType::get(builder.getVoidTy(), false)};

  llvm::Function *mainFunction{llvm::Function::Create(
      funcType, llvm::Function::ExternalLinkage, "main", module.get())};

  llvm::BasicBlock *entry{
      llvm::BasicBlock::Create(context, "entry", mainFunction)};

  builder.SetInsertPoint(entry);

  // My test
  llvm::LLVMContext *contextPtr{&context};

  const std::filesystem::path path{"../test/test1.nc"};
  std::shared_ptr<Sintax> sintax{std::make_shared<NicoleSintax>()};
  const std::shared_ptr<Parser> parser{std::make_shared<TopDown>(sintax)};
  const auto result{parser->parse(path)};
  
  auto tree{result.get()};
  // if (argc > 1) {
  PrintTree printer{};
  // std::cout << printer.print(tree) << "\n";
  //  return 0;
  // }
  CodeGeneration codeGen{contextPtr, module.get(), entry};
  llvm::Value *returnValue{codeGen.generate(tree)};
  /*
    if (!returnValue) {
      std::cerr << "Error: No return value generated." << std::endl;
      return 1;
    }

    builder.CreateRet(returnValue);
  */
  // Verificar el módulo y la función main
  llvm::verifyFunction(*mainFunction);
  llvm::verifyModule(*module);

  // Imprimir el IR generado
  module->print(llvm::errs(), nullptr);
  ////////////////////
  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();
  llvm::InitializeNativeTargetAsmParser();
  std::string errStr;
  llvm::ExecutionEngine *execEngine =
      llvm::EngineBuilder(std::move(module))
          .setErrorStr(&errStr)
          .setOptLevel(llvm::CodeGenOptLevel::Default)
          .create();
  if (!execEngine) {
    std::cerr << "Failed to create ExecutionEngine: " << errStr << std::endl;
    return 1;
  }

  // Ejecuta la función main y obtiene el resultado
  std::vector<llvm::GenericValue> noargs;
  llvm::GenericValue gv = execEngine->runFunction(mainFunction, noargs);
  // Imprime el resultado
  // char resultChar = static_cast<std::string>(gv.IntVal.getZExtValue());
  // std::cout << "Result: " << resultChar << std::endl;

  /*
  PARA STRINGS
  char* resultPtr = (char*)gv.PointerVal;
  std::cout << "Result: " << resultPtr << std::endl;
  */

  delete execEngine;

  return 0;
}
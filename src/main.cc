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

// Just creates a main function for our program like a wrapper
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
  PrintTree printer{};
  // std::cout << printer.print(tree) << "\n";
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

/*
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
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/FormattedStream.h>
#include <llvm/Support/raw_ostream.h>


#include "../inc/parsingAnalysis/parsingAlgorithms/topDown.h"
#include "../inc/visitors/codeGeneration.h"
#include "../inc/visitors/printTree.h"
#include "llvm/Support/ErrorHandling.h"

using namespace nicole;

// Just creates a main function for our program like a wrapper
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
      funcType, llvm::Function::ExternalLinkage, "main2", module.get())};

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
  PrintTree printer{};
  CodeGeneration codeGen{contextPtr, module.get(), entry};
  llvm::Value *returnValue{codeGen.generate(tree)};
  
  // Verificar el módulo y la función main
  llvm::verifyFunction(*mainFunction);
  llvm::verifyModule(*module);

  // Imprimir el IR generado
  module->print(llvm::errs(), nullptr);

  auto TargetTriple = llvm::sys::getDefaultTargetTriple();
  llvm::InitializeAllTargetInfos();
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmParsers();
  llvm::InitializeAllAsmPrinters();

  std::string Error;
  auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);

  // Print an error and exit if we couldn't find the requested target.
  // This generally occurs if we've forgotten to initialise the
  // TargetRegistry or we have a bogus target triple.
  if (!Target) {
    llvm::errs() << Error;
    return 1;
  }
  auto CPU = "generic";
  auto Features = "";

  llvm::TargetOptions opt;
  auto TargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, llvm::Reloc::PIC_);
  
  module->setDataLayout(TargetMachine->createDataLayout());
  module->setTargetTriple(TargetTriple);

  auto Filename = "output.o";
  std::error_code EC;
  llvm::raw_fd_ostream dest(Filename, EC, llvm::sys::fs::OF_None);

  if (EC) {
    llvm::errs() << "Could not open file: " << EC.message();
    return 1;
  }

  llvm::legacy::PassManager pass;
  auto FileType = llvm::CodeGenFileType::ObjectFile;

  if (TargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
    llvm::errs() << "TargetMachine can't emit a file of this type";
    return 1;
  }

  pass.run(*module);
  dest.flush();

  
  return 0;
}

 */
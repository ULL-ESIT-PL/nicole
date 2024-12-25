#include "../inc/lexicalAnalysis/nicoleSintax.h"
#include "../inc/options/optionsParser.h"
#include <cstdlib>

// Just creates a main function for our program like a wrapper
int main(int argc, char *argv[]) {
  const std::vector<std::string_view> arguments(argv + 1, argv + argc);
  std::expected<nicole::Options, nicole::Error> options{
      nicole::OptionParser::parse(arguments)};

  if (!options) {
    std::cerr << options.error() << "\n";
    return 1;
  } else if (options->help()) {
    // Mostrar help
    return EXIT_SUCCESS;
  }

  const std::shared_ptr<nicole::Sintax> sintax{
      std::make_shared<nicole::NicoleSintax>()}; 
  const nicole::Lexer lexer{sintax->createLexer()};
  const std::expected<nicole::TokenStream, nicole::Error> result{
      lexer.analyze(options->entryFilePath())};

  if (!result) {
    std::cerr << result.error() << "\n";
    return 2;
  }

  for (const auto &token : *result) {
    std::cout << "Type: " << nicole::tokenTypeToString(token.type())
              << " ---> Raw: " << token.raw()
              << " ---> Loc: " << token.locInfo() << "\n";
  }
  return 0;
}

/*
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/FormattedStream.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/TargetParser/Host.h>


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
  auto TargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features,
opt, llvm::Reloc::PIC_);

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
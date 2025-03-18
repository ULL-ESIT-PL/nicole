#include "../inc/lexicalAnalysis/nicoleSintax.h"
#include "../inc/options/optionsParser.h"
#include "../inc/parsingAnalysis/algorithm/topDown.h"
#include "../inc/visitors/printTree.h"
#include "../inc/visitors/validateTree.h"
#include "../inc/visitors/fillSemanticInfo.h"
#include "../inc/visitors/monomorphize.h"
#include "../inc/visitors/typeAnalysis.h"
#include "../inc/visitors/codeGeneration.h"
#include <iostream>
#include <memory>

void helper() noexcept {
  std::cout
      << "Usage \n"
         "From the parent directory, run:\n"
         "\t./nicole.sh [[options] input_file] | "
         "-t Where input_file is the main program file with the "
         ".nc extension(e.g., helloWorld.nc).\n\n"
         "Options can appear in any position except -n, "
         "which must be followed by the output file name.\n\n"
         "\t-h | --help --> Displays a brief description of how to use the "
         "compiler.\n\n"
         "\t-v | --validate --> Forces the program to follow certain "
         "validation "
         "rules (recommended).\n\n"
         "\t-o | --optimize --> Performs optimizations on the generated "
         "code.\n\n"
         "\t-n | --name output_file --> Allows specifying the name of the "
         "output "
         "file (default is a.out).\n\n"
         "\t-p | --printTree --> Prints the Abstract Syntax Tree (AST) in a "
         "directory-like structure.\n\n"
         "\t-i | --printIR --> Prints the generated Intermediate "
         "Representation "
         "(IR) code.\n\n"
         "\t-t --> Executes the tests of the compiler, also to run the tests "
         "no "
         "other argument but -t can be passed to the script.\n\n"
         "Usage Examples:\n"
         "\t- Compile a file with optimization and validation, specifying the "
         "output executable name:\n"
         "      ./nicole.sh -v -o -n program_out helloWorld.nc\n\n"
         "\t- Generate the AST and intermediate code without optimization :\n"
         "      ./nicole.sh -p -i helloWorld.nc\n";
}

// Just creates a main function for our program like a wrapper
int main(int argc, char *argv[]) {
  const std::vector<std::string_view> arguments(argv + 1, argv + argc);
  std::expected<nicole::Options, nicole::Error> options{
      nicole::OptionsParser::parse(arguments)};

  if (!options) {
    std::cerr << options.error() << "\n";
    return 1;
  } else if (options->help()) {
    helper();
    return EXIT_SUCCESS;
  }

  const std::shared_ptr<nicole::Sintax> sintax{
      std::make_shared<nicole::NicoleSintax>()};

  const nicole::TopDown topDown{sintax};

  const std::expected<std::shared_ptr<nicole::Tree>, nicole::Error> tree{
      topDown.parse(options->entryFilePath())};

  if (!tree) {
    std::cerr << tree.error() << "\n" << std::flush;
    return 2;
  }

  if (options->validateTree()) {
    const nicole::ValidateTree validateTree{};
    const std::expected<bool, nicole::Error> validated{
        validateTree.validate((*tree).get())};

    if (!validated) {
      std::cerr << validated.error() << "\n" << std::flush;
      return 3;
    }
  }

  if (options->printTree()) {
    const nicole::PrintTree printTree{};
    const std::expected<std::string, nicole::Error> toStr{
        printTree.print((*tree).get())};
    if (!toStr) {
      std::cout << toStr.error();
      return 4;
    }
    std::cout << *toStr << "\n";
  }

  std::shared_ptr<nicole::EnumTable> enumTable{std::make_shared<nicole::EnumTable>()};
  std::shared_ptr<nicole::FunctionTable> functionTable{std::make_shared<nicole::FunctionTable>()};
  std::shared_ptr<nicole::TypeTable> typeTable{std::make_shared<nicole::TypeTable>()};
  const nicole::FillSemanticInfo semanticFiller{enumTable, functionTable, typeTable, options->validateTree()};
  const auto isTableFilled{semanticFiller.fill((*tree).get())};
  if (!isTableFilled) {
    std::cout << isTableFilled.error() << "\n" << std::flush;
    return 5;
  }
  return EXIT_SUCCESS;
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
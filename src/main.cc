#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>

#include <iostream>
#include <string>

#include "../inc/parsingAnalysis/parsingAlgorithms/parser.h"
#include "../inc/parsingAnalysis/parsingAlgorithms/topDown.h"
#include "../inc/visitors/codeGeneration.h"
using namespace nicole;

int main() {
  // Start LLVM
  llvm::LLVMContext context;
  llvm::IRBuilder<> builder(context);
  std::unique_ptr<llvm::Module> module =
      std::make_unique<llvm::Module>("my_module", context);
  std::map<std::string, llvm::Value*> namedValues;

  // Crear una función main y un bloque básico
  llvm::FunctionType* funcType =
      llvm::FunctionType::get(builder.getInt8PtrTy(), false);
  llvm::Function* mainFunction = llvm::Function::Create(
      funcType, llvm::Function::ExternalLinkage, "main", module.get());
  llvm::BasicBlock* entry =
      llvm::BasicBlock::Create(context, "entry", mainFunction);
  builder.SetInsertPoint(entry);

  // My test
  llvm::LLVMContext* contextPtr{&context};
  /*
  NodeLiteralString lit1{contextPtr, module.get(), "HOLA"};
  NodeLiteralString* left{&lit1};

  llvm::Value* leftEvaluated = left->codeGeneration();
  builder.CreateRet(leftEvaluated);

  NodeLiteralChar lit2{contextPtr, 'k'};
  NodeLiteralChar* right{&lit2};
  llvm::Value* rightEvaluated = right->codeGeneration();
  builder.CreateRet(rightEvaluated);

  // NodeBinary bii{contextPtr, left, Operator::ADD, right};
  // NodeBinary* result{&bii};
  //  llvm::Value* resultEvaluated = result->codeGeneration();
  builder.CreateRet(leftEvaluated);
  */
  const std::filesystem::path path{"../test/test1.nc"};
  const std::unique_ptr<Parser> parser{
      std::make_unique<TopDown>()};
  const auto result{parser->parse(path)};
  CodeGeneration codeGen{contextPtr, module.get()};
  Visitor* visitor{&codeGen};
  // builder.CreateRet(result->codeGeneration());
  auto resultCasted = dynamic_cast<NodeStatement*>(result.get());
  builder.CreateRet(visitor->visit(resultCasted));

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
  llvm::ExecutionEngine* execEngine = llvm::EngineBuilder(std::move(module))
                                          .setErrorStr(&errStr)
                                          .setOptLevel(llvm::CodeGenOpt::None)
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
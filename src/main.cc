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

#include "../inc/lexer/lexer.h"
#include "../inc/parser/nodeLiteral.h"
using namespace nicole;

int main() {
  const Lexer lexer{std::vector<Category>{
      Category{TokenType::SPACE, "\\s+", true},
      Category{TokenType::COMMENT, "\\/\\*(.|\\n)*?\\*\\/", true},

      Category{TokenType::NUMBER, "\\d+(\\.\\d+)?([eE][-]?\\d+)?", false},
      Category{TokenType::STRING, "\"(?:\\\\.|[^\"\\\\])*\"", false},
      Category{TokenType::TRUE, "true", false},
      Category{TokenType::FALSE, "false", false},

      Category{TokenType::CONST, "const", false},

      Category{TokenType::IMPORT, "import", false},

      Category{TokenType::PRINT, "print", false},
      Category{TokenType::SYSTEM, "system", false},
      Category{TokenType::TYPE, "type", false},

      Category{TokenType::IF, "if", false},
      Category{TokenType::ELSE, "else", false},
      Category{TokenType::WHILE, "while", false},
      Category{TokenType::FOR, "for", false},
      Category{TokenType::STOP, "stop", false},
      Category{TokenType::PASS, "pass", false},
      Category{TokenType::RETURN, "return", false},
      Category{TokenType::FUNCTION, "def", false},

      Category{TokenType::OR, "or", false},
      Category{TokenType::AND, "and", false},

      Category{TokenType::ID, "[a-zA-Z]+[a-zA-Z0-9]*", false},

      Category{TokenType::RP, "\\)", false},
      Category{TokenType::LP, "\\(", false},
      Category{TokenType::RB, "\\}", false},
      Category{TokenType::LB, "\\{", false},
      Category{TokenType::RC, "\\]", false},
      Category{TokenType::LC, "\\[", false},
      Category{TokenType::SEMICOLON, ";", false},
      Category{TokenType::COMMA, ",", false},
      Category{TokenType::TERNARY, "\\?", false},
      Category{TokenType::DOTDOT, ":", false},

      Category{TokenType::SMALLEREQUAL, "<=", false},
      Category{TokenType::BIGGEREQUAL, ">=", false},
      Category{TokenType::NOTEQUAL, "!=", false},
      Category{TokenType::EQUAL, "==", false},

      Category{TokenType::ASSIGNMENT, "=", false},

      Category{TokenType::INCREMENT, "\\+\\+|\\-\\-", false},
      Category{TokenType::OPERATOR, "[+-/\\*<>!%]", false},
  }};
  const std::filesystem::path path{"../test/test1.nc"};
  const auto tokens{lexer.analyze(path, true)};

  // Start LLVM
  llvm::LLVMContext context;
  llvm::IRBuilder<> builder(context);
  std::unique_ptr<llvm::Module> module =
      std::make_unique<llvm::Module>("my_module", context);
  std::map<std::string, llvm::Value*> namedValues;

  // Crear una función main y un bloque básico
  llvm::FunctionType* funcType =
      llvm::FunctionType::get(builder.getDoubleTy(), false);
  llvm::Function* mainFunction = llvm::Function::Create(
      funcType, llvm::Function::ExternalLinkage, "main", module.get());
  llvm::BasicBlock* entry =
      llvm::BasicBlock::Create(context, "entry", mainFunction);
  builder.SetInsertPoint(entry);

  // My test
  llvm::LLVMContext* contextPtr{&context};
  shared_ptr<NodeLiteral> lit{make_shared<NodeLiteral>(contextPtr, 10.6)};
  llvm::Value* retVal = lit->codeGeneration();
  builder.CreateRet(retVal);

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
  std::cout << "Result: " << gv.DoubleVal << std::endl;
  delete execEngine;

  return 0;
}
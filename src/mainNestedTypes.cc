#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/CodeGen.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <iostream>

int main() {
    // Inicializar el entorno de LLVM
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    // Crear el contexto y el módulo
    llvm::LLVMContext Context;
    std::unique_ptr<llvm::Module> Module = std::make_unique<llvm::Module>("StructExample", Context);
    llvm::IRBuilder<> Builder(Context);

    // Definir la estructura 'A' con un entero 'x'
    llvm::StructType *StructA = llvm::StructType::create(Context, "A");
    StructA->setBody(llvm::Type::getInt32Ty(Context));

    // Definir la estructura 'B' que contiene una instancia de 'A' llamada 'y'
    llvm::StructType *StructB = llvm::StructType::create(Context, "B");
    StructB->setBody(StructA);

    // Crear una función 'main' con retorno int y sin parámetros
    llvm::FunctionType *FuncType = llvm::FunctionType::get(llvm::Type::getInt32Ty(Context), false);
    llvm::Function *MainFunc = llvm::Function::Create(FuncType, llvm::Function::ExternalLinkage, "main", Module.get());
    llvm::BasicBlock *EntryBB = llvm::BasicBlock::Create(Context, "entry", MainFunc);
    Builder.SetInsertPoint(EntryBB);

    // Reservar espacio para una instancia de 'B'
    llvm::AllocaInst *VarB = Builder.CreateAlloca(StructB, nullptr, "b");

    // Acceder al campo 'y' de 'b'
    llvm::Value *PtrY = Builder.CreateStructGEP(StructB, VarB, 0, "y");

    // Acceder al campo 'x' de 'y'
    llvm::Value *PtrX = Builder.CreateStructGEP(StructA, PtrY, 0, "x");

    // Asignar un valor al campo 'x'
    Builder.CreateStore(llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 42), PtrX);

    // Declarar la función printf
    llvm::FunctionType *PrintfType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(Context), llvm::PointerType::get(llvm::Type::getInt8Ty(Context), 0), true);
    llvm::FunctionCallee PrintfFunc = Module->getOrInsertFunction("printf", PrintfType);

    // Crear la cadena de formato "%d\n"
    llvm::Value *FormatStr = Builder.CreateGlobalStringPtr("%d\n");

    // Cargar el valor de 'x'
    llvm::Value *ValueX = Builder.CreateLoad(llvm::Type::getInt32Ty(Context), PtrX, "x_val");

    // Llamar a printf con el valor de 'x'
    Builder.CreateCall(PrintfFunc, {FormatStr, ValueX});

    // Retornar 0
    Builder.CreateRet(llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 0));

    // Verificar el módulo
    llvm::verifyModule(*Module, &llvm::errs());

    // Imprimir el IR generado
    Module->print(llvm::outs(), nullptr);

    // Configurar el motor de ejecución
    std::string ErrStr;
    llvm::ExecutionEngine *EE = llvm::EngineBuilder(std::move(Module))
                                    .setErrorStr(&ErrStr)
                                    .setOptLevel(llvm::CodeGenOptLevel::Default)
                                    .create();
    if (!EE) {
        std::cerr << "Error al crear ExecutionEngine: " << ErrStr << std::endl;
        return 1;
    }

    // Ejecutar la función 'main'
    std::vector<llvm::GenericValue> Args;
    llvm::GenericValue GV = EE->runFunction(MainFunc, Args);

    // Liberar recursos
    delete EE;

    return 0;
}

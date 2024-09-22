#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/InitLLVM.h>
#include <llvm/Support/ManagedStatic.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/IR/DerivedTypes.h>

using namespace llvm;

int main() {
    // Inicializar el contexto de LLVM
    LLVMContext Context;
    Module *ModuleOb = new Module("main_module", Context);
    IRBuilder<> Builder(Context);

    // Definir el tipo de la función main
    FunctionType *FuncType = FunctionType::get(Type::getInt32Ty(Context), false);
    Function *MainFunc = Function::Create(FuncType, Function::ExternalLinkage, "main", ModuleOb);

    // Crear un bloque básico para la función
    BasicBlock *BB = BasicBlock::Create(Context, "entry", MainFunc);
    Builder.SetInsertPoint(BB);

    // Declarar las variables a y b
    AllocaInst *a = Builder.CreateAlloca(Type::getInt32Ty(Context), nullptr, "a");
    AllocaInst *b = Builder.CreateAlloca(Type::getInt32Ty(Context), nullptr, "b");

    // Inicializar a y b
    Builder.CreateStore(ConstantInt::get(Type::getInt32Ty(Context), 5), a);
    Builder.CreateStore(ConstantInt::get(Type::getInt32Ty(Context), 7), b);

    // Cargar a y b
    Value *val_a = Builder.CreateLoad(Type::getInt32Ty(Context), a, "load_a");
    Value *val_b = Builder.CreateLoad(Type::getInt32Ty(Context), b, "load_b");

    // Sumar a y b
    Value *result = Builder.CreateAdd(val_a, val_b, "sum_result");

    // Llamar a printf
    FunctionType *printfType = FunctionType::get(Type::getInt32Ty(Context), 
                            PointerType::get(Type::getInt8Ty(Context), 0), true);
    FunctionCallee printfFunc = ModuleOb->getOrInsertFunction("printf", printfType);
    Value *fmtStr = Builder.CreateGlobalStringPtr("La suma de %d y %d es %d\n");

    // Crear los argumentos para printf
    std::vector<Value*> printfArgs;
    printfArgs.push_back(fmtStr);
    printfArgs.push_back(val_a); // valor de a
    printfArgs.push_back(val_b); // valor de b
    printfArgs.push_back(result); // resultado

    // Llamar a printf con los argumentos correctos
    Builder.CreateCall(printfFunc, printfArgs);

    // Retornar 0
    Builder.CreateRet(ConstantInt::get(Type::getInt32Ty(Context), 0));

    // Imprimir el módulo en formato LLVM IR
    ModuleOb->print(outs(), nullptr);

    // Ejecutar el código
    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();
    ExecutionEngine *EE = EngineBuilder(std::unique_ptr<Module>(ModuleOb)).create();

    std::vector<GenericValue> Args;
    EE->runFunction(MainFunc, Args);

    return 0;
}

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Support/raw_ostream.h"
#include <memory>

int main() {
    llvm::LLVMContext Context;
    auto Module = std::make_unique<llvm::Module>("my_module", Context);

    llvm::FunctionType *FuncType = llvm::FunctionType::get(llvm::Type::getVoidTy(Context), false);
    llvm::Function *Function = llvm::Function::Create(FuncType, llvm::Function::ExternalLinkage, "my_function", *Module);

    llvm::BasicBlock *EntryBB = llvm::BasicBlock::Create(Context, "entry", Function);
    llvm::IRBuilder<> Builder(EntryBB);

    llvm::Type *Int32Type = llvm::Type::getInt32Ty(Context);
    llvm::AllocaInst *Alloca = Builder.CreateAlloca(Int32Type, nullptr, "my_var");

    llvm::Value *Value = llvm::ConstantInt::get(Int32Type, 10);
    Builder.CreateStore(Value, Alloca);

    llvm::Value *LoadedValue = Builder.CreateLoad(Int32Type, Alloca, "loaded_var");

    Builder.CreateRetVoid();

    Module->print(llvm::outs(), nullptr);

    return 0;
}

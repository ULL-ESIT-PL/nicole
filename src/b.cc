#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

int main() {
    // Crear el contexto LLVM
    LLVMContext context;
    IRBuilder<> builder(context);

    // Crear un módulo LLVM
    auto module = std::make_unique<Module>("my_module", context);

    // Crear la función foo con un tipo de retorno void
    FunctionType* funcType = FunctionType::get(Type::getVoidTy(context), false);
    Function* fooFunction = Function::Create(funcType, Function::ExternalLinkage, "foo", module.get());

    // Crear un bloque básico y establecer el punto de inserción
    BasicBlock* entry = BasicBlock::Create(context, "entry", fooFunction);
    builder.SetInsertPoint(entry);

    // Crear un literal entero
    Value* intLiteral = ConstantInt::get(builder.getInt32Ty(), 1);

    // Puedes hacer algo con el literal entero aquí. En este caso, simplemente se inserta.
    // Esto simula la inserción del literal en la función, pero no realiza ninguna operación con él.

    // Retornar de la función (no se devuelve valor porque la función es void)
    builder.CreateRetVoid();

    // Verificar el módulo y la función foo
    if (verifyFunction(*fooFunction, &errs())) {
        errs() << "Error: Function verification failed\n";
        return 1;
    }
    if (verifyModule(*module, &errs())) {
        errs() << "Error: Module verification failed\n";
        return 1;
    }

    // Imprimir el IR generado
    module->print(llvm::errs(), nullptr);

    return 0;
}

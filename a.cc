#include <llvm/Support/Error.h>
#include <llvm/Support/ErrorHandling.h>
#include <iostream>

llvm::Expected<int> bar(int x) {
    if (x < 0) {
        return llvm::make_error<llvm::StringError>("Error: x no puede ser negativo", llvm::inconvertibleErrorCode());
    }
    return x + 42;
}

void process(int x) {
    if (x < 0) {
        llvm::report_fatal_error("Error fatal: x no puede ser negativoooooooo");
    }
    std::cout << "Procesando x: " << x << std::endl;
}

int main() {
    auto result = bar(1);
    if (!result) {
        std::cerr << "Ocurrió un error: " << toString(result.takeError()) << std::endl;
        return 1;
    }
    std::cout << "Resultado: " << *result << std::endl;
    process(-1);
    return 0;
}

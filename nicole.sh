#!/bin/bash

# Comprobar si Ninja está instalado y definir el generador
if command -v ninja &> /dev/null; then
    GENERATOR="-G Ninja"
else
    GENERATOR=""
fi

# Crear el directorio build si no existe
if [ ! -d "./build" ]; then
    mkdir build
    cd build
    # cmake ..
    cmake $GENERATOR ..
    cmake --build . --parallel $(nproc)
    cd ..
fi

# Comprobar si se pasa la opción -t
if [[ "$1" == "-t" ]]; then
    cd build

    cmake $GENERATOR -DENABLE_COVERAGE=ON ..

    cmake --build . --parallel $(nproc)

    # Extraer el compilador usado desde CMakeCache.txt
    
    echo "Usando llvm-cov (Clang)"
    # Exportar variable para que Clang genere archivos .profraw
    export LLVM_PROFILE_FILE="coverage_%p.profraw"
    # Ejecutar los tests para generar archivos .profraw
    ctest --verbose
    # Fusionar los archivos .profraw en un único archivo de datos
    llvm-profdata merge -sparse coverage_*.profraw -o coverage.profdata
    # Generar un reporte en formato lcov (para usar luego genhtml) usando el binario de tests
    llvm-cov export ./bin/tests -instr-profile=coverage.profdata -format=lcov > coverage.info

    genhtml coverage.info --output-directory ../coverage

    echo "Informe de cobertura generado en la carpeta 'coverage'."

    exit 0
fi

# Compilar el proyecto
cd build
cmake --build . --parallel $(nproc)

# Volver al directorio principal
cd ..

# Ejecutar el binario principal con los argumentos proporcionados
./build/bin/binaries "$@"

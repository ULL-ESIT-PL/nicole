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

 # Extraer o detectar el compilador
    if [ -f CMakeCache.txt ]; then
        COMPILER=$(grep "CMAKE_CXX_COMPILER:" CMakeCache.txt | head -n1 | cut -d'=' -f2)
    else
        echo "CMakeCache.txt no encontrado. Detectando compilador manualmente..."
        if command -v clang++ &> /dev/null; then
            COMPILER="clang++"
        elif command -v g++ &> /dev/null; then
            COMPILER="g++"
        else
            echo "No se encontró un compilador compatible."
            exit 1
        fi
    fi
    echo "Compilador utilizado: $COMPILER"

    if echo "$COMPILER" | grep -qi "clang"; then
        echo "Usando llvm-cov (Clang)"
        export LLVM_PROFILE_FILE="coverage_%p.profraw"
        ctest --verbose
        llvm-profdata merge -sparse coverage_*.profraw -o coverage.profdata
        llvm-cov export ./bin/tests -instr-profile=coverage.profdata -format=lcov > coverage.info
    elif echo "$COMPILER" | grep -qi "g++"; then
        echo "Usando gcov (GCC)"
        ctest --verbose
        lcov --capture --directory . --output-file coverage.info
    else
        echo "Compilador desconocido. No se puede generar cobertura."
        exit 1
    fi

    genhtml coverage.info --output-directory ../docs/coverage
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

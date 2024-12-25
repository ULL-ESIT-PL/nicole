#!/bin/bash

# Crear el directorio build si no existe
if [ ! -d "./build" ]; then
    mkdir build
    cd build
    cmake ..
    make
    cd ..
fi

# Comprobar si se pasa la opción -t
if [[ "$1" == "-t" ]]; then
    cd build
    make
    # Ejecutar los tests
    ctest --verbose
    exit 0
fi

# Compilar el proyecto
cd build
make

# Volver al directorio principal
cd ..

# Ejecutar el binario principal con los argumentos proporcionados
./build/bin/binaries "$@"

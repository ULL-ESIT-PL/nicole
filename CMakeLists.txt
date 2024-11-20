# Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Detectar y configurar el compilador
if(NOT DEFINED CMAKE_CXX_COMPILER)
    find_program(CLANG_COMPILER clang++)
    if(CLANG_COMPILER)
        set(CMAKE_CXX_COMPILER "${CLANG_COMPILER}" CACHE STRING "C++ compiler" FORCE)
    else()
        find_program(GNU_COMPILER g++)
        if(GNU_COMPILER)
            set(CMAKE_CXX_COMPILER "${GNU_COMPILER}" CACHE STRING "C++ compiler" FORCE)
        else()
            message(FATAL_ERROR "No se encontró un compilador compatible (clang++ o g++).")
        endif()
    endif()
endif()

# Project name
project(MyProject LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Check if Clang is being used, and if not, set it as the default
#if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#    set(CMAKE_CXX_COMPILER "clang++")
#endif()

# Get LLVM configuration using llvm-config
execute_process(
    COMMAND llvm-config --cxxflags
    OUTPUT_VARIABLE LLVM_CXX_FLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND llvm-config --ldflags
    OUTPUT_VARIABLE LLVM_LDFLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND llvm-config --system-libs
    OUTPUT_VARIABLE LLVM_SYSTEM_LIBS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND llvm-config --libs core
    OUTPUT_VARIABLE LLVM_LIBS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Set compiler and linker flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3 ${LLVM_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LLVM_LDFLAGS} ${LLVM_SYSTEM_LIBS} ${LLVM_LIBS}")

# Source files
set(SRC_FILES
    src/main.cc
    src/lexicalAnalysis/type.cc
    src/lexicalAnalysis/tokenStream.cc
    src/lexicalAnalysis/lexer.cc
    src/lexicalAnalysis/nicoleSintax.cc
    src/parsingAnalysis/ast/nodeType.cc
    src/parsingAnalysis/ast/literals/nodeLiteralChar.cc
    src/parsingAnalysis/ast/literals/nodeLiteralString.cc
    src/parsingAnalysis/ast/declaration/varTable.cc
    src/parsingAnalysis/ast/declaration/functionTable.cc
    src/parsingAnalysis/ast/utils/convert.cc
    src/parsingAnalysis/types/typeTable.cc
    src/parsingAnalysis/parsingAlgorithms/topDown.cc
    src/parsingAnalysis/parsingAlgorithms/productions/parseBinary.cc
    src/parsingAnalysis/parsingAlgorithms/productions/parseFactor.cc
    src/parsingAnalysis/parsingAlgorithms/productions/parseUtils.cc
    src/parsingAnalysis/parsingAlgorithms/productions/parseLoop.cc
    src/parsingAnalysis/parsingAlgorithms/productions/parseConditional.cc
    src/parsingAnalysis/parsingAlgorithms/productions/parseDecl.cc
    src/visitors/codeGen/codeGeneration.cc
    src/visitors/codeGen/codeGenFactor.cc
    src/visitors/codeGen/codeGenLoop.cc
    src/visitors/codeGen/codeGenConditional.cc
    src/visitors/codeGen/codeGenBinary.cc
    src/visitors/codeGen/codeGenDecl.cc
    src/visitors/codeGen/codeGenCall.cc
    src/visitors/codeGen/codeGenUtils.cc
    src/visitors/printTree.cc
)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)

# Executable
add_executable(binaries ${SRC_FILES})

# Link LLVM libraries
target_link_libraries(binaries ${LLVM_LIBS} ${LLVM_SYSTEM_LIBS})

# Custom targets
add_custom_target(run
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/binaries
    DEPENDS binaries
    COMMENT "Running the program..."
)

add_custom_target(ignore
    COMMAND git add .gitignore
    COMMENT "Adding .gitignore file to git..."
)

# Clean command
add_custom_target(cln
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/bin/obj
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/bin
    COMMENT "Cleaning up..."
)

# Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name
project(MyProject LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
set(CMAKE_CXX_COMPILER "clang++")

# Get LLVM configuration
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

    src/lexicalAnalysis/lexer.cc
    src/lexicalAnalysis/nicoleSintax.cc

    src/parsingAnalysis/literals/nodeLiteralDouble.cc
    src/parsingAnalysis/literals/nodeLiteralInt.cc
    src/parsingAnalysis/literals/nodeLiteralBool.cc
    src/parsingAnalysis/literals/nodeLiteralChar.cc
    src/parsingAnalysis/literals/nodeLiteralString.cc

    src/parsingAnalysis/operations/nodeBinary.cc

    src/parsingAnalysis/statements/statement.cc
    
    src/parsingAnalysis/parsingAlgorithms/parser.cc
    src/parsingAnalysis/parsingAlgorithms/topDown.cc

    src/visitors/codeGeneration.cc
)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)

# Executable
add_executable(binaries ${SRC_FILES})

# Custom commands
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

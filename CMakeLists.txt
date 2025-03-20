# Minimum CMake version
cmake_minimum_required(VERSION 3.30.5)

# usar en la linea de comandos para generar compile_commands.json y que .clangd detecte catch2
# cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .

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

project(MyProject LANGUAGES CXX)

# Configurar ccache (si está instalado)
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    message(STATUS "Usando ccache: ${CCACHE_PROGRAM}")
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
endif()

# C++ standard y optimizaciones
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Activate LTO optimization
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

# Colored erros
set(CMAKE_COLOR_DIAGNOSTICS ON)

# Configurar warnings
option(ENABLE_WARNINGS "Activar flags de warnings" ON)
if(ENABLE_WARNINGS)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(COMPILER_WARNINGS "-Wall -Wextra -Werror -Wpedantic \
                               -Wshadow -Wnon-virtual-dtor -Wold-style-cast \
                               -Wcast-align -Wunused -Woverloaded-virtual \
                               -Wconversion -Wsign-conversion -Wnull-dereference \
                               -Wdouble-promotion -Wformat=2 -Wmisleading-indentation")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(COMPILER_WARNINGS "-Wall -Wextra -Werror -Wpedantic \
                               -Wshadow -Wnon-virtual-dtor -Wold-style-cast \
                               -Wcast-align -Wunused -Woverloaded-virtual \
                               -Wconversion -Wsign-conversion -Wnull-dereference \
                               -Wdouble-promotion -Wformat=2 -Wmisleading-indentation \
                               -Wduplicated-cond -Wduplicated-branches -Wlogical-op \
                               -Wuseless-cast")
    else()
        message(WARNING "No se aplicaron flags específicos para el compilador.")
    endif()
else()
    set(COMPILER_WARNINGS "")
endif()

# Directorios de salida
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

###########################################################
# catch2
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.7.0
)
FetchContent_MakeAvailable(Catch2)

add_executable(tests
    # Lista de archivos para tests
    test/errors/test_error.cc
    test/options/test_options.cc
    test/options/test_optionsParser.cc
    src/options/optionsParser.cc
    # lexicalAnalysis
    test/lexicalAnalysis/test_category.cc
    test/lexicalAnalysis/test_lexer.cc
    src/lexicalAnalysis/lexer.cc
    src/lexicalAnalysis/type.cc
    test/lexicalAnalysis/test_location.cc
    test/lexicalAnalysis/test_regexWrapper.cc
    test/lexicalAnalysis/test_sintax.cc
    src/lexicalAnalysis/nicoleSintax.cc
    test/lexicalAnalysis/test_token.cc
    test/lexicalAnalysis/test_tokenStream.cc
    src/lexicalAnalysis/tokenStream.cc
    test/lexicalAnalysis/test_type.cc
    # parsingAnalysis
    src/parsingAnalysis/ast/astType.cc 
    src/parsingAnalysis/builder.cc
    src/parsingAnalysis/checkPosition.cc
    src/parsingAnalysis/algorithm/topDown.cc
    src/parsingAnalysis/algorithm/productions/parseBinary.cc
    src/parsingAnalysis/algorithm/productions/parseConditionals.cc
    src/parsingAnalysis/algorithm/productions/parseDecl.cc
    src/parsingAnalysis/algorithm/productions/parseFactor.cc
    src/parsingAnalysis/algorithm/productions/parseFunctions.cc
    src/parsingAnalysis/algorithm/productions/parseLoops.cc
    src/parsingAnalysis/algorithm/productions/parseSelfAssignment.cc
    src/parsingAnalysis/algorithm/productions/parseUserTypes.cc
    src/parsingAnalysis/algorithm/productions/parseUtils.cc
    src/parsingAnalysis/algorithm/productions/parseChained.cc
    src/parsingAnalysis/algorithm/productions/parseTypes.cc
    test/parsingAnalysis/test_ast_literals.cc
    test/parsingAnalysis/test_ast_vector.cc 
    test/parsingAnalysis/test_ast_variables.cc 
    test/parsingAnalysis/test_ast_utils.cc 
    test/parsingAnalysis/test_ast_statements.cc 
    test/parsingAnalysis/test_ast_assignment.cc 
    test/parsingAnalysis/test_ast_unary.cc 
    test/parsingAnalysis/test_ast_binary.cc
    test/parsingAnalysis/test_ast_conditionals.cc
    test/parsingAnalysis/test_ast_loops.cc
    test/parsingAnalysis/test_ast_functions.cc
    test/parsingAnalysis/test_ast_pointers.cc

    # tables
    src/tables/functionTable/functionTable.cc
    src/tables/functionTable/function.cc
    src/tables/typeTable/typeTable.cc
    src/tables/typeTable/userType.cc
    src/tables/typeTable/method.cc
    src/tables/typeTable/methodTable.cc
    src/tables/typeTable/attrTable.cc
    src/tables/scope/scope.cc

    # visitors
    src/visitors/printTree.cc
    src/visitors/validateTree.cc
    src/visitors/fillSemanticInfo.cc
    src/visitors/monomorphize.cc
    src/visitors/typeAnalysis.cc
    src/visitors/codeGeneration.cc

    # compiler
    src/compiler/nicole.cc
)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_compile_options(tests PRIVATE
    -Wno-error=implicit-int-conversion
    -Wno-error=old-style-cast
    -Wno-error=sign-conversion
)

# Habilitar coverage para tests si se activa la opción ENABLE_COVERAGE
option(ENABLE_COVERAGE "Habilitar flags de coverage para tests" OFF)
if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message(STATUS "Coverage habilitado con GCC")
        target_compile_options(tests PRIVATE --coverage)
        target_link_options(tests PRIVATE --coverage)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message(STATUS "Coverage habilitado con Clang")
        target_compile_options(tests PRIVATE -fprofile-instr-generate -fcoverage-mapping)
        target_link_options(tests PRIVATE -fprofile-instr-generate -fcoverage-mapping)
    else()
        message(WARNING "Coverage solo soportado para GCC o Clang")
    endif()
endif()

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(tests)

###########################################################
# Configuración de LLVM
option(USE_LLVM_CONFIG_19 "Utilizar llvm-config-19 en lugar de llvm-config" OFF)
if(USE_LLVM_CONFIG_19)
    set(LLVM_CONFIG "llvm-config-19")
else()
    set(LLVM_CONFIG "llvm-config")
endif()

execute_process(
    COMMAND  ${LLVM_CONFIG} --cxxflags
    OUTPUT_VARIABLE LLVM_CXX_FLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
    COMMAND  ${LLVM_CONFIG} --ldflags
    OUTPUT_VARIABLE LLVM_LDFLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
    COMMAND  ${LLVM_CONFIG} --system-libs
    OUTPUT_VARIABLE LLVM_SYSTEM_LIBS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
    COMMAND  ${LLVM_CONFIG} --libs core
    OUTPUT_VARIABLE LLVM_LIBS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
    COMMAND  ${LLVM_CONFIG} --includedir
    OUTPUT_VARIABLE LLVM_INCLUDEDIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

include_directories(${LLVM_INCLUDEDIR})

# Set compiler and linker flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3 ${COMPILER_WARNINGS} ${LLVM_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LLVM_LDFLAGS} ${LLVM_SYSTEM_LIBS} ${LLVM_LIBS}")

# Source files para el ejecutable principal
set(SRC_FILES
    src/main.cc
    src/options/optionsParser.cc
    src/lexicalAnalysis/type.cc
    src/lexicalAnalysis/tokenStream.cc
    src/lexicalAnalysis/lexer.cc
    src/lexicalAnalysis/nicoleSintax.cc
    src/parsingAnalysis/ast/astType.cc
    src/parsingAnalysis/builder.cc
    src/parsingAnalysis/checkPosition.cc
    src/parsingAnalysis/algorithm/topDown.cc
    src/parsingAnalysis/algorithm/productions/parseBinary.cc
    src/parsingAnalysis/algorithm/productions/parseConditionals.cc
    src/parsingAnalysis/algorithm/productions/parseDecl.cc
    src/parsingAnalysis/algorithm/productions/parseFactor.cc
    src/parsingAnalysis/algorithm/productions/parseFunctions.cc
    src/parsingAnalysis/algorithm/productions/parseLoops.cc
    src/parsingAnalysis/algorithm/productions/parseSelfAssignment.cc
    src/parsingAnalysis/algorithm/productions/parseUserTypes.cc
    src/parsingAnalysis/algorithm/productions/parseUtils.cc
    src/parsingAnalysis/algorithm/productions/parseChained.cc
    src/parsingAnalysis/algorithm/productions/parseTypes.cc

    src/tables/functionTable/functionTable.cc
    src/tables/functionTable/function.cc
    src/tables/typeTable/typeTable.cc
    src/tables/typeTable/userType.cc
    src/tables/typeTable/method.cc
    src/tables/typeTable/methodTable.cc
    src/tables/typeTable/attrTable.cc
    src/tables/scope/scope.cc

    src/visitors/printTree.cc
    src/visitors/validateTree.cc
    src/visitors/fillSemanticInfo.cc
    src/visitors/monomorphize.cc
    src/visitors/typeAnalysis.cc
    src/visitors/codeGeneration.cc

    src/compiler/nicole.cc
)

# Include directories para el ejecutable principal
include_directories(${CMAKE_SOURCE_DIR}/src)

# Executable principal
add_executable(binaries ${SRC_FILES})
target_include_directories(binaries PRIVATE ${LLVM_INCLUDEDIR})
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
add_custom_target(cln
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/bin/obj
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/bin
    COMMENT "Cleaning up..."
)

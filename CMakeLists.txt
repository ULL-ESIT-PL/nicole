# Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name
project(MyProject LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
set(CMAKE_CXX_COMPILER "clang++")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -pedantic -O3 `llvm-config --cxxflags --ldflags --system-libs --libs core`")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -pedantic -O3")

# Source files
set(SRC_FILES
    src/main.cc
)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include directories (if needed)
include_directories(${CMAKE_SOURCE_DIR}/src)

# Executable
add_executable(binaries ${SRC_FILES})

# Link libraries
target_link_libraries(binaries PRIVATE boost_regex)

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

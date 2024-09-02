#include <exception>
#include <iostream>
#include <string>

// clang++ -o my_compiler a.cc `llvm-config --cxxflags --ldflags --system-libs
// --libs core`

int main(int argc, char* argv[]) {
    throw std::runtime_error(std::to_string(argc));
}
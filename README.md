# nicole

Toy Compiler written in c++ using LLVM by Evian Concepción Peña

## Goals

Combine an user friendly sintax and the speed of a compiled language, example: Mojo (uses MLIR) or Julia
WARNING: Currently translation to machine code is commented, to develope faster I use the JIT engine of LLVM

## How to build

- At parent directory --> ```mkdir build```
- At parent directory --> ```cd build```
- At build directory --> ```cmake ..```
- At nicole directory --> ```nicole.sh file.nc```
- ```nicole.sh``` will compile and excute but only will compile those modified files
- Still no options from command line so test/test1.nc is the program to be compiled

## Features Implemented

- Lexer (sintax is up to the users since the parser only knows about token types and not raw information)

- Parsing algorithm is up to users meaning it can easily be changed, for this project I just implement a Top-Down

- Tree visualization

- Literals Bool, Int, Float, Double, Char, String

- Variable Declaration

- Const Variable Declaration

- Unary operations -- (decrement), ++ (increment), -, ! (not)

- Binary operations or, and, +, -, *, /, %, !=, ==, <, <=, >, >=

- - Operators +=, -=, *=, /=

- Operator precedency and parenthesis expressions

- Variable call

- Function Declaration with return (type / void) and allowing recursion

- Function calls

- If conditional with else as optional

- Ternary operator

- Switch conditional with optional default (still gotta check that every case has the right type)

- Loops while, do while, for

- Every time a function, loop or conditional is being created it inherits the outer scope

- Print in the terminal (by te moment just literal and not user types)

- Import (still gotta check if every path works and more tests about dependency cycles)

- Structures of data (can access either as a getter or to set the attributes after constructor, just missing methods and nested types)

## Features yet to implement

- Better error messages (missing print more than one error per compilation)

- Auto declaration

- Vectors / Array

- Classes

- Stop (equivalent to break in c++)

- Pass (equivalent to continue in c++)

- Pointers and null

- Concatenation of expression, example: variable[index][index].method... Right now would require create an intermediate variable

- Standar library

- Type conversion

- Type checking analysis

- Optimization analysis (let the user decide between differnt levels, Default, None, Agressive...)

- System calls

- Garbage recollector for variable lifetime

- Threads (not sure)

- Templates (not sure)

- Polymorphism (not sure)

- NameSpace (not sure)

- Use MLIR -> Multi Level Intermediate Representation (not sure)


## Folder structure

- The .txt files next to the CMakeLists.txt are just meant for me to install correctly LLVM and the intellisense being able to find the library

- Building system (cmake, in the futute might add Bazel)

- inc for every header file (.h)

    - The folders are divide by stages (since this is an early stage of the project it might not be accurate the file path)
    - 1. Lexical Analysis (has everything related to building a lexer)
    - 2. Parsing Analaysis (has everything related to de parsing, AST nodes, tables)
    - 3. Type checking (not implemnted)
    - 4. Optimization analysis (not implemented)
    - 5. Visitors (Code generation)

- src for every source file (.cc)

- test currenty not used (expected to use google test library)

- build where the binary is
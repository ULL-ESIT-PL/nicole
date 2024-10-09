# nicole

Toy Compiler written in c++ using LLVM by Evian Concepción Peña

## How to build

- At parent directory --> ```mkdir build```
- At parent directory --> ```cd build```
- At build directory --> ```cmake ..```
- At build directory --> ```make run```
- ```make run``` will compile and excute but only will compile those modified files
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

- Operator precedency and parenthesis expressions

- Variable call

- Function Declaration with return (type / void) and allowing recursion

- Function calls

- If conditional with else as optional

- Loops while, for

- Every time a function, loop or conditional is being created it inherits the outer scope

- Print in the terminal (by te moment just literal and not user types)

## Features yet to implement

- Better error messages

- Vectors / Array

- Structures of data

- Classes

- Stop (equivalent to break in c++)

- Pass (equivalent to continue in c++)

- Pointers and null

- Ternary operator

- Operators +=, -=...

- Concatenation of expression, example: variable[index][index].method...

- Import / Export

- Standar library

- Type conversion

- Type checking analysis

- Optimization analysis

- System calls

- Garbage recollector for variable lifetime

- Threads (not sure)

- Templates (not sure)

- Polymorphism (not sure)

- Use MLIR -> Multi Level Intermediate Representation (not sure)


## Folder structure

### I'm aware of the spaghetti code in the parsing algorithm and lack of a few pattern designs and comments, that by lack of time still have not being used.

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
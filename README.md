# Nicole Compiler

![mascot](images/image.png)

Toy Compiler written in c++ using LLVM 19 by Evian Concepción Peña

Tutor: Casiano Rodríguez León

Cotutor: Jose Rodriguez

## Description
`nicole.sh` is a script that compiles files with the `.nc` extension. If changes have been made to the source code, only the modified files will be recompiled.

## Usage
From the parent directory, run:
```sh
./nicole.sh [[options] input_file] | -t
```
Where `input_file` is the main program file with the `.nc` extension (e.g., `helloWorld.nc`).

## Options
Options can appear in any position except `-n`, which must be followed by the output file name.

- `-h | --help` → Displays a brief description of how to use the compiler.
- `-v | --validate` → Forces the program to follow certain validation rules (recommended).
- `-o | --optimize` → Performs optimizations on the generated code.
- `-n | --name output_file` → Allows specifying the name of the output file (default is `a.out`).
- `-p | --printTree` → Prints the Abstract Syntax Tree (AST) in a directory-like structure.
- `-i | --printIR` → Prints the generated Intermediate Representation (IR) code.

- `-t` → Executes the tests of the compiler, also to run the tests no other argyment but `-t`can be passed to the script.

## Usage Examples
Compile a file with optimization and validation, specifying the output executable name:
```sh
./nicole.sh -v -o -n program_out helloWorld.nc
```

Generate the AST and intermediate code without optimization:
```sh
./nicole.sh -p -i helloWorld.nc
```

## Notes
- Only the file containing the `main` function needs to be specified.
- If changes are made to the code (project), only the modified files will be recompiled.
- `-n` must be followed by the output file name.
- A few examples of the languaje can be found at `nicoleScripts/`

## Requirements
- `clang 19 or g++ 14`
- `Catch2 3.7`
- `LLVM 19`
- `cmake 3.3`
- `ninja 1.12` for faster compilation
- `lcov 2.0.1` for coverage analysis
- `The project only has been tested in Linux`

## Features Implemented

## Features yet to implement

## Folder structure
#!/bin/bash

if [ ! -d "./build" ]; then
    mkdir build
    cd build
    cmake ..
    make run
fi

cd build

make

cd ..

./build/bin/binaries "$1"
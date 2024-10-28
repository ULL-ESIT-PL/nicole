#!/bin/bash

cd build

make

cd ..

./build/bin/binaries "$1"
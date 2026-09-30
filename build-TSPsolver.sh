#!/bin/bash

set -e

# Create build directory if it does not exist

cd tsp-solver

mkdir -p build

# Run CMake and compile inside build/
cd build
cmake ..
make
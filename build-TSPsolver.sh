#!/bin/bash
set -e

# Go to the project folder from any dir
cd "$(dirname "$0")/tsp-solver"

mkdir -p build


cd build


cmake ..

# Compile 
cmake --build .
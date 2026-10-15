#!/bin/bash
set -e

# Work from the project root, whatever the current directory is
cd "$(dirname "$0")"

# Configure (tests included) and compile into ./build
cmake -S tsp-solver -B build -DBUILD_TESTS=ON
cmake --build build

#!/bin/bash
set -e

cd "$(dirname "$0")"

cmake -S tsp-solver -B build -DBUILD_TESTS=ON
cmake --build build

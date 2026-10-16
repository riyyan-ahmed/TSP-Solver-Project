# TSP Solver

C++ solver for the Travelling Salesman Problem (M1 AI programming project, UGA).

# Group Members
Riyyan Ahmed  & Salaheddine Moustafid

## Build

bash
bash build-TSPsolver.sh


Creates build/TSPsolver and build/tests. Needs CMake 3.16+, a C++20 compiler and internet access the first time (GoogleTest is downloaded).

## Run

bash
./build/TSPsolver <instance_path> <output_path> [-m <algorithm>]


Algorithms: trivial (default).

Example: ./build/TSPsolver TSP-instances/lower_diag/gr17.tsp gr17.tour

## Tests

bash
./build/tests


## Project structure


build-TSPsolver.sh       build script
TSP-instances/           provided instances (coord/ and lower_diag/)
instance_generator.py    random instance generator
docs/                    project subject and TSPLIB format
tsp-solver/
  CMakeLists.txt
  include/               headers
  src/                   sources
  tests/                 GoogleTest tests


| Class | Files | Role |
|---|---|---|
| TspParser | TspParser.cpp, LowerDiagParser.cpp, CoordinateParser.cpp | reads a .tsp file into a TspInstance |
| TspInstance | TspInstance.cpp | name, city ids and distance graph |
| Graph, MatrixGraph, CoordinatesGraph | Graph.cpp, MatrixGraph.cpp, CoordinatesGraph.cpp | distances between cities |
| Solver, TrivialSolver | Algorithms.hpp, Trivial.cpp | solving methods |
| TourWriter | TourWriter.cpp | writes the tour file |
| — | main.cpp | command line: parse, solve, write |

## Adding an algorithm

1. Declare a class deriving from Solver in Algorithms.hpp and implement solve() in a new file in src/.
2. Add its name to makeSolver() in main.cpp.

New .cpp files in src/ and tests/ are picked up by CMake automatically.

# Cmake Project Template

This is a simple CMake project template that demonstrates how to set up a basic C++ project using CMake as the build system. It includes a sample source file and a CMake configuration file.

## Project Structure
```
CmakeProjectTemplate/
├── CMakeLists.txt
|── include/
│   └── utils.hpp
├── src/
|   ├── utils.cpp    
│   └── main.cpp
├── tests/
|   └── tests.cpp
└── README.md
```

All source files are located in the `src` directory, header files in the `include` directory, and test files in the `tests` directory.

## Getting Started

### Prerequisites

- CMake (version 3.10 or higher)
- A C++ compiler (e.g., GCC, Clang, MSVC)
- Google Test framework for unit testing

### Building the Project

Run the following commands in your terminal:

```bash
mkdir build
cd build
cmake ..
make
```

Now the project should be built, and the executable will be located in the `build` directory.
Run the following command to execute the application:

```bash
./build/main
``` 

Alternatively, after building, you can compile and run the application directly from the root directory using:

```bash
cmake --build build
./build/main
```

### Building and Running Tests

To build and run the tests, use the following commands:

```bash
cd build
cmake -DBUILD_TESTS=ON ..
make
./build/tests
```
This will compile the test files and run the tests using the Google Test framework.

### Debugging

To build the project in debug mode, you can specify the build type when running CMake:

```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
```


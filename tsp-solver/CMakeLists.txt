cmake_minimum_required(VERSION 3.14)
project(my_project)

# --- Global options ---
# Use C++20 standard, change if needed
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Debug/Release configuration ---
# By default, build in Release mode
# To set the build type to Debug, use:
#      cmake -DCMAKE_BUILD_TYPE=Debug ..
# ------------------------------------
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# --- Compiler flags ---
# Warning flags for all build types
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
# Debugging flags for Debug build
set(CMAKE_CXX_FLAGS_DEBUG "-g")
# Optimization flags for Release build
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# --- Directories ---
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)

# --- Source variables ---
file(GLOB SOURCES "${SOURCE_DIR}/*.cpp")
file(GLOB HEADERS "${INCLUDE_DIR}/*.hpp")

# Remove main.cpp from SOURCES
# We must do so because main.cpp is not part of the library
# and should not be compiled into it.
# Instead, it will be compiled into the main executable.
# Add here any other source files that should be excluded from the library.
list(REMOVE_ITEM SOURCES "${SOURCE_DIR}/main.cpp")

# --- Including libraries ---
add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})

# --- Main executable ---
add_executable(main ${SOURCE_DIR}/main.cpp)
target_link_libraries(main PRIVATE ${PROJECT_NAME})


# --- External dependencies ---
# Example with Boost program options (uncomment if needed)
# find_package(Boost REQUIRED COMPONENTS program_options)
# target_link_libraries(${PROJECT_NAME} PRIVATE Boost::boost)

# --- Tests (optionnal) ---
if(BUILD_TESTS)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.17.0
  )
  FetchContent_MakeAvailable(googletest)

  enable_testing()
  file(GLOB TEST_SOURCES "${TEST_DIR}/*.cpp")
  add_executable(tests ${TEST_SOURCES})
  target_link_libraries(tests PRIVATE ${PROJECT_NAME} GTest::gtest_main)
  target_include_directories(tests PRIVATE ${INCLUDE_DIR})
  add_test(NAME tests COMMAND tests)
endif()



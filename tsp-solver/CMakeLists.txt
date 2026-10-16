cmake_minimum_required(VERSION 3.16)
project(tsp_solver)

# --- Global options ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Release by default (use -DCMAKE_BUILD_TYPE=Debug to debug) ---
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# --- Compiler flags ---
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
# Only applied to our targets: GoogleTest does not compile with -Werror
set(WARNING_FLAGS -Wall -Wextra -Werror)

# --- Directories ---
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)

# --- Sources (main.cpp only belongs to the executable) ---
file(GLOB SOURCES "${SOURCE_DIR}/*.cpp")
file(GLOB HEADERS "${INCLUDE_DIR}/*.hpp")
list(REMOVE_ITEM SOURCES "${SOURCE_DIR}/main.cpp")

# --- Library ---
add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})
target_compile_options(${PROJECT_NAME} PRIVATE ${WARNING_FLAGS})

# --- Executable: build/TSPsolver ---
add_executable(TSPsolver ${SOURCE_DIR}/main.cpp)
target_link_libraries(TSPsolver PRIVATE ${PROJECT_NAME})
target_compile_options(TSPsolver PRIVATE ${WARNING_FLAGS})

# --- Tests: build/tests (enabled with -DBUILD_TESTS=ON) ---
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
  target_compile_options(tests PRIVATE ${WARNING_FLAGS})
  # Paths the tests need to find the instance files and the executable
  target_compile_definitions(tests PRIVATE
    TSP_INSTANCES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../TSP-instances"
    TSP_SOLVER_EXE="$<TARGET_FILE:TSPsolver>")
  add_dependencies(tests TSPsolver)
  add_test(NAME tests COMMAND tests)
endif()

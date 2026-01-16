cmake_minimum_required(VERSION 3.10)
project(ZeroLockAllocator CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Thread support
find_package(Threads REQUIRED)

# Source files
set(ALLOCATOR_SOURCES
    src/allocator.cpp
    src/allocate.cpp
)

# Include directories
include_directories(include)

# Library
add_library(allocator ${ALLOCATOR_SOURCES})
target_link_libraries(allocator Threads::Threads)

# Test executables
add_executable(test_threads experiments/test_threads.cpp)
target_link_libraries(test_threads allocator Threads::Threads)

add_executable(benchmark benchmarks/benchmark.cpp)
target_link_libraries(benchmark allocator Threads::Threads)

# Test target
enable_testing()
add_test(NAME ThreadSafety COMMAND test_threads)
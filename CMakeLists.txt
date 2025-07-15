cmake_minimum_required(VERSION 3.16)
project(HighPerformanceAllocator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Allocator sources
file(GLOB ALLOCATOR_SRC
    "src/allocator/*.cpp"
    "src/utils/*.cpp"
)

add_library(high_performance_allocator STATIC ${ALLOCATOR_SRC})

target_include_directories(high_performance_allocator PUBLIC include)

# Example executable (basic usage)
add_executable(basic_usage examples/basic_usage.cpp)
target_link_libraries(basic_usage PRIVATE high_performance_allocator)

# Create test directories if they do not exist
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/tests)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/tests/unit_tests)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/tests/integration_tests)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/tests/stress_tests)

# Add test targets (placeholders)
add_subdirectory(tests/unit_tests EXCLUDE_FROM_ALL)
add_subdirectory(tests/integration_tests EXCLUDE_FROM_ALL)
add_subdirectory(tests/stress_tests EXCLUDE_FROM_ALL)

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

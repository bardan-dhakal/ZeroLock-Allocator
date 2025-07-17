# High-Performance Lock-Free Memory Allocator

A modern C++ project for a high-performance, lock-free memory allocator system supporting multiple allocation strategies. Designed for Big Tech, Finance/Quant, and Embedded systems.

## Features
- Multiple allocation strategies: Pool, Lock-Free, Stack, Buddy, Slab
- Lock-free, thread-safe design (std::atomic, CAS)
- Thread-local and NUMA-aware optimizations
- Memory alignment and cache optimization
- Comprehensive benchmarking and testing
- Modern C++17/20 features

## Project Status

### Implemented
- Project structure and directory scaffolding
- **PoolAllocator**: Minimal fixed-size block allocator (with unit test)
- Minimal code for HighPerformanceAllocator and LockFreeAllocator

### Stubs (Empty or Incomplete)
- StackAllocator, BuddyAllocator, SlabAllocator (headers and sources)
- Utility headers and sources (atomic_utils, memory_utils, benchmark_utils)
- Benchmarks (all .cpp files)
- Most examples (except basic_usage.cpp)
- All documentation in docs/

### Missing
- All test directories and files (unit, integration, stress) except PoolAllocator unit test
- Full implementations for all allocators except PoolAllocator
- Complete documentation
- Comprehensive examples and benchmarks

## Usage Example

```cpp
#include "allocator/pool_allocator.hpp"
#include <iostream>

int main() {
    PoolAllocator pool(32, 4); // 4 blocks of 32 bytes
    void* a = pool.allocate();
    void* b = pool.allocate();
    pool.deallocate(a);
    void* c = pool.allocate(); // c == a (reused)
    std::cout << "PoolAllocator working!" << std::endl;
}
```

## How to Build

```sh
mkdir build && cd build
cmake ..
make
```

## How to Test

```sh
cd build
./unit_tests
```

## How to Contribute
See ProjectDoc.txt for the full specification and roadmap. Contributions are welcome to any of the above areas!

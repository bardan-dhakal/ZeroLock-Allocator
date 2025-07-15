# High-Performance Lock-Free Memory Allocator

This project implements a high-performance, lock-free memory allocator in modern C++. It supports multiple allocation strategies and is designed for use in high-performance, multi-threaded, and embedded systems.

## Features
- Lock-free allocator using atomic operations
- Pool, stack, buddy, and slab allocators
- Thread-local optimizations
- NUMA-aware strategies
- Comprehensive benchmarking and testing

## Build Instructions

```
mkdir build && cd build
cmake ..
make
```

## Usage
See `examples/basic_usage.cpp` for a simple usage example.

## Project Structure
- `include/` - Public headers
- `src/` - Implementation files
- `benchmarks/` - Benchmarking tools
- `tests/` - Unit, integration, and stress tests
- `examples/` - Example usage for different domains
- `docs/` - Design and API documentation

## Project Status

### Implemented
- Project structure and directory scaffolding
- Minimal code for HighPerformanceAllocator and LockFreeAllocator

### Stubs (Empty or Incomplete)
- PoolAllocator, StackAllocator, BuddyAllocator, SlabAllocator (headers and sources)
- Utility headers and sources (atomic_utils, memory_utils, benchmark_utils)
- Benchmarks (all .cpp files)
- Most examples (except basic_usage.cpp)
- All documentation in docs/

### Missing
- All test directories and files (unit, integration, stress)
- Full implementations for all allocators except LockFreeAllocator
- Complete documentation
- Comprehensive examples and benchmarks

### How to Contribute
See ProjectDoc.txt for the full specification and roadmap. Contributions are welcome to any of the above areas!

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

# Lock-Free Memory Allocator

Learning project implementing lock-free synchronization for memory allocation.

## What I Built
- Lock-free free list using `std::atomic<T>` and compare-and-swap
- CAS retry loops for thread-safe pop/push operations  
- Multi-threaded testing with 4+ concurrent threads
- Benchmark framework comparing against malloc

## Building

### Using CMake (Recommended):
```bash
mkdir build && cd build
cmake ..
make
./test_threads
```

### Using g++ directly:
```bash
g++ -std=c++20 -pthread experiments/test_threads.cpp src/allocator.cpp src/allocate.cpp -o test_threads
```

## Key Implementation
```cpp
// Lock-free pop using CAS retry loop
do {
    expected = head.load();
    if (!expected) return nullptr;
    new_head = expected->next;
} while (!head.compare_exchange_weak(expected, new_head));
```

## Technical Concepts Demonstrated
- Atomic operations and memory ordering
- Compare-and-swap for lock-free synchronization
- Race condition prevention without mutexes
- Concurrent data structure design

## Benchmark Results
Current implementation: ~8-15M ops/sec across thread counts, improving efficiency is in progress, expected boost after adding Thread Local Caching.


## Learning Outcomes
- Deep understanding of atomics and CAS
- Experience with concurrent programming challenges
- Knowledge of memory allocator architecture
- Understanding of performance tradeoffs in lock-free design

## Next Steps If Continued
- Add thread-local caching (major performance improvement)
- Implement multiple size classes
- Add ABA problem mitigation
- Larger memory pools


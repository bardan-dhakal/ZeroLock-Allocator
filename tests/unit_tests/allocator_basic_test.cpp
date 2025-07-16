#include <cassert>
#include <iostream>
#include "allocator/pool_allocator.hpp"

int main() {
    std::cout << "Unit test: PoolAllocator basic test\n";
    PoolAllocator pool(32, 4); // 4 blocks of 32 bytes
    void* a = pool.allocate();
    void* b = pool.allocate();
    void* c = pool.allocate();
    void* d = pool.allocate();
    void* e = pool.allocate(); // Should be nullptr (out of memory)
    assert(a && b && c && d);
    assert(e == nullptr);
    pool.deallocate(b);
    void* f = pool.allocate();
    assert(f == b); // Should reuse freed block
    std::cout << "PoolAllocator basic test passed.\n";
    return 0;
} 
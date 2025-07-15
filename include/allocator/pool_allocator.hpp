#pragma once

class PoolAllocator {
public:
    PoolAllocator();
    void* allocate(size_t size);
    void deallocate(void* ptr);
};

#pragma once
#include <cstddef>

class SlabAllocator {
public:
    SlabAllocator();
    void* allocate(size_t size);
    void deallocate(void* ptr);
};

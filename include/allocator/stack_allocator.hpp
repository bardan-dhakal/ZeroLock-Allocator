#pragma once
#include <cstddef>

class StackAllocator {
public:
    StackAllocator();
    void* allocate(size_t size);
    void deallocate(void* ptr);
};

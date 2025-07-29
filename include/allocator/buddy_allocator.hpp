#pragma once
#include <cstddef>

class BuddyAllocator {
public:
    BuddyAllocator();
    void* allocate(size_t size);
    void deallocate(void* ptr);
};

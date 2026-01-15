#pragma once
#include "allocator.hpp"
#include <cstddef>

namespace LockFreeAllocator
{
    void* allocate(size_t size);
    void free(void* ptr);
}


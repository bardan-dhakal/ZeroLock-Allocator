#include "allocator/slab_allocator.hpp"
#include <cstddef>

SlabAllocator::SlabAllocator() {}

void* SlabAllocator::allocate(size_t size) {
    return nullptr;
}

void SlabAllocator::deallocate(void* ptr) {
    // stub
}

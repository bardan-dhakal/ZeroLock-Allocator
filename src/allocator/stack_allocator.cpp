#include "allocator/stack_allocator.hpp"
#include <cstddef>

StackAllocator::StackAllocator() {}

void* StackAllocator::allocate(size_t size) {
    return nullptr;
}

void StackAllocator::deallocate(void* ptr) {
    // stub
}

#include "allocator/pool_allocator.hpp"
#include <cstdlib>
#include <iostream>
#include <cassert>

PoolAllocator::PoolAllocator(size_t block_size, size_t block_count) {
    // Initialize member variables
    block_size_ = block_size;
    block_count_ = block_count;

    // Allocate memory pool and check for failure
    memory_ = malloc(block_size * block_count);
    assert(memory_ != nullptr && "malloc failed");

    // Build linked list of free blocks
    void* moving_ptr = memory_;

    for (size_t i = 0; i < block_count; i++)
    {
        if (i == block_count - 1)
        {
            // Last block points to null (end of list)
            *reinterpret_cast<void**>(moving_ptr) = nullptr;
            break;
        }
        
        // Each block points to the next block
        *reinterpret_cast<void**>(moving_ptr) = *reinterpret_cast<char**>(moving_ptr) + block_size;
        moving_ptr = static_cast<char*>(moving_ptr) + block_size;
    }

    // Initialize free list to point to first block
    free_list_ = memory_;
}

PoolAllocator::~PoolAllocator(){
    free(memory_);
}

void* PoolAllocator::allocate(){
    if (free_list_ == nullptr) {
        return nullptr; // Out of memory
    }
    
    // Take first block from free list
    void* block = free_list_;
    free_list_ = *reinterpret_cast<void**>(free_list_); // Move to next block

    return block;
}

void PoolAllocator::deallocate(void* ptr) {
    if (ptr == nullptr) {
        return; // Ignore null pointers
    }
    
    // Add block back to front of free list
    *reinterpret_cast<void**>(ptr) = free_list_;
    free_list_ = ptr;
}

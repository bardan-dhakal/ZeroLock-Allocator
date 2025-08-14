#include "allocator/pool_allocator.hpp"
#include <cstdlib>
#include <iostream>
#include <cassert>

PoolAllocator::PoolAllocator(size_t block_size, size_t block_count) {
    block_size_ = block_size;
    block_count_ = block_count;

    memory_ = malloc(block_size * block_count);
    assert(memory_ != nullptr && "malloc failed");

    
    void* moving_ptr = memory_;

    for (size_t i = 0; i < block_count; i++)
    {
        if (i == block_count - 1)
        {
            *reinterpret_cast<void**>(moving_ptr) = nullptr;
            break;
        }
        
        *reinterpret_cast<void**>(moving_ptr) = moving_ptr + block_size;
        moving_ptr = static_cast<char*>(moving_ptr) + block_size;
    }

    free_list_ = memory_;
}

PoolAllocator::~PoolAllocator(){
    free(memory_);
}

void* PoolAllocator::allocate(){
    if (free_list_ == nullptr) {
        return nullptr;
    }
    void* block = free_list_;
    free_list_ = *reinterpret_cast<void**>(free_list_);

    return block;
    
}

void PoolAllocator::deallocate(void* ptr) {
    if (ptr == nullptr) {
        return;
    }
    *reinterpret_cast<void**>(ptr) = free_list_;
    free_list_ = ptr;
}

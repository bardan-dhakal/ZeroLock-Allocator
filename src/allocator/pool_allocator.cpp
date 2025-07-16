#include "allocator/pool_allocator.hpp"
#include <cstdlib>
#include <cassert>
#include <new>

PoolAllocator::PoolAllocator(size_t block_size, size_t block_count)
    : block_size_(block_size), block_count_(block_count), memory_(nullptr), free_list_(nullptr) {
    // Ensure block size is at least pointer size
    if (block_size_ < sizeof(void*)) block_size_ = sizeof(void*);
    memory_ = std::malloc(block_size_ * block_count_);
    assert(memory_ && "PoolAllocator: malloc failed");
    // Initialize free list
    char* p = static_cast<char*>(memory_);
    for (size_t i = 0; i < block_count_; ++i) {
        void* next = (i < block_count_ - 1) ? (p + block_size_) : nullptr;
        *reinterpret_cast<void**>(p) = next;
        p += block_size_;
    }
    free_list_ = memory_;
}

PoolAllocator::~PoolAllocator() {
    std::free(memory_);
}

void* PoolAllocator::allocate() {
    if (!free_list_) return nullptr; // Out of memory
    void* block = free_list_;
    free_list_ = *reinterpret_cast<void**>(free_list_);
    return block;
}

void PoolAllocator::deallocate(void* ptr) {
    if (!ptr) return;
    *reinterpret_cast<void**>(ptr) = free_list_;
    free_list_ = ptr;
}

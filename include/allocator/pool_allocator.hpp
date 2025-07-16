#pragma once
#include <cstddef>
#include <vector>

class PoolAllocator {
public:
    PoolAllocator(size_t block_size, size_t block_count);
    ~PoolAllocator();
    void* allocate();
    void deallocate(void* ptr);

private:
    size_t block_size_;
    size_t block_count_;
    void* memory_;
    void* free_list_;
};

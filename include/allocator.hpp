#pragma once
#include <cstddef>
#include <atomic>
#include "BlockHeader.hpp"

namespace LockFreeAllocator
{
    extern std::atomic<BlockHeader*> free_list_head;

    constexpr size_t PAGE_SIZE = 4096;
    constexpr size_t PAYLOAD = 64;


    void initialize_allocator();
    void* allocate_raw_memory();

}


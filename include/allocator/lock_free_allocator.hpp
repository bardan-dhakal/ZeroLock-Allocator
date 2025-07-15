#pragma once
#include <atomic>
#include <cstddef>
#include <cstdint>

struct Node {
    std::atomic<Node*> next;
    alignas(64) char data[];
};

class LockFreeAllocator {
    std::atomic<Node*> free_list_;
public:
    LockFreeAllocator();
    ~LockFreeAllocator();
    void* allocate();
    void deallocate(void* ptr);
private:
    void* allocate_new_chunk();
};

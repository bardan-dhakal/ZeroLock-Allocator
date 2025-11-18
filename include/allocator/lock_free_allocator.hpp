#pragma once
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <vector>

struct Node {
    std::atomic<Node*> next;
    // Remaining space for user data (determined at runtime)
};

// Internal chunk header for cleanup
struct ChunkHeader {
    void* memory;
    struct ChunkHeader* next;
};

class LockFreeAllocator {
    std::atomic<Node*> free_list_;
    ChunkHeader* chunk_list_;           // Singly-linked list of allocated chunks
    size_t block_size_;                 // Size of each node (header + data)
    size_t blocks_per_chunk_;           // How many blocks in each chunk

public:
    // block_size: Total size per block (e.g., 256 bytes, includes Node header)
    // blocks_per_chunk: How many blocks to allocate at once (e.g., 1000)
    LockFreeAllocator(size_t block_size = 256, size_t blocks_per_chunk = 1000);
    ~LockFreeAllocator();

    void* allocate();
    void deallocate(void* ptr);

private:
    void* allocate_new_chunk();
};

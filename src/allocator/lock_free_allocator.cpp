#include "../../include/allocator/lock_free_allocator.hpp"
#include <cstdlib>
#include <cassert>
#include <cstring>

LockFreeAllocator::LockFreeAllocator(size_t block_size, size_t blocks_per_chunk)
    : free_list_(nullptr), chunk_list_(nullptr),
      block_size_(block_size), blocks_per_chunk_(blocks_per_chunk) {
    // Validate block size (must be at least size of Node)
    assert(block_size >= sizeof(Node));

    // Allocate initial chunk
    allocate_new_chunk();
}

LockFreeAllocator::~LockFreeAllocator() {
    // Free all allocated chunks
    ChunkHeader* chunk = chunk_list_;
    while (chunk != nullptr) {
        ChunkHeader* next = chunk->next;
        free(chunk->memory);
        delete chunk;
        chunk = next;
    }
}

void* LockFreeAllocator::allocate() {
    Node* head = free_list_.load(std::memory_order_acquire);

    // Try to claim a block from the free list using CAS
    while (head != nullptr) {
        Node* next = head->next.load(std::memory_order_relaxed);

        if (free_list_.compare_exchange_weak(head, next,
                std::memory_order_release, std::memory_order_acquire)) {
            // Successfully claimed this block
            // Return pointer to the data portion (skip the Node header)
            return reinterpret_cast<char*>(head) + sizeof(Node);
        }
        // CAS failed, head was updated, retry
    }

    // Free list is empty, allocate a new chunk and retry
    if (allocate_new_chunk() != nullptr) {
        return allocate();  // Retry allocation
    }

    return nullptr;  // Allocation failed
}

void LockFreeAllocator::deallocate(void* ptr) {
    if (ptr == nullptr) {
        return;
    }

    // The ptr points to data. The Node header is just before it.
    // We allocated sizeof(Node) bytes for the header before the data.
    Node* node = reinterpret_cast<Node*>(
        reinterpret_cast<char*>(ptr) - sizeof(Node)
    );

    // Lock-free deallocation: push this node back to the front of free list
    Node* head = free_list_.load(std::memory_order_acquire);

    while (true) {
        // Set this node's next to the current head
        node->next.store(head, std::memory_order_relaxed);

        // Try to make this node the new head of free list
        if (free_list_.compare_exchange_weak(head, node,
                std::memory_order_release, std::memory_order_acquire)) {
            // Successfully returned block to free list
            return;
        }
        // CAS failed because head changed, loop and retry
    }
}

void* LockFreeAllocator::allocate_new_chunk() {
    // Allocate memory for all blocks in this chunk
    char* chunk_memory = static_cast<char*>(malloc(block_size_ * blocks_per_chunk_));

    if (chunk_memory == nullptr) {
        return nullptr;
    }

    // Get the first node in this chunk
    Node* first_node = reinterpret_cast<Node*>(chunk_memory);

    // Build a linked list of free blocks within this chunk
    for (size_t i = 0; i < blocks_per_chunk_; ++i) {
        Node* current = reinterpret_cast<Node*>(chunk_memory + i * block_size_);

        if (i == blocks_per_chunk_ - 1) {
            // Last node in chunk: will point to current free list head
            // We'll set this below after loading the head
            current->next.store(nullptr, std::memory_order_relaxed);
        } else {
            // Point to next node in chunk
            Node* next = reinterpret_cast<Node*>(chunk_memory + (i + 1) * block_size_);
            current->next.store(next, std::memory_order_relaxed);
        }
    }

    // Atomically prepend all these blocks to the global free list
    // Get the current head
    Node* head = free_list_.load(std::memory_order_acquire);

    while (true) {
        // Set the last node in the chunk to point to the current head
        Node* last_node = reinterpret_cast<Node*>(chunk_memory + (blocks_per_chunk_ - 1) * block_size_);
        last_node->next.store(head, std::memory_order_relaxed);

        // Try to make the first node of this chunk the new head
        if (free_list_.compare_exchange_weak(head, first_node,
                std::memory_order_release, std::memory_order_acquire)) {
            break;  // Success
        }
        // Head changed, retry
    }

    // Track this chunk for cleanup
    ChunkHeader* chunk_hdr = new ChunkHeader();
    chunk_hdr->memory = chunk_memory;
    chunk_hdr->next = chunk_list_;
    chunk_list_ = chunk_hdr;

    return chunk_memory;
}

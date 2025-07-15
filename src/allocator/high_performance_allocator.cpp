#include "../../include/allocator/high_performance_allocator.hpp"
#include <cstdlib>

// Template methods must be defined in the header, so only non-template methods here

void* HighPerformanceAllocator::allocate_bytes(std::size_t bytes, std::size_t alignment) {
    // Stub: use aligned_alloc for now
    return std::aligned_alloc(alignment, bytes);
}

void HighPerformanceAllocator::deallocate_bytes(void* ptr, std::size_t /*bytes*/, std::size_t /*alignment*/) {
    std::free(ptr);
}

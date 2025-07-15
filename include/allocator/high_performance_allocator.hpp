#pragma once
#include <cstddef>
#include <type_traits>

class HighPerformanceAllocator {
public:
    template<typename T>
    T* allocate(std::size_t count = 1) {
        void* ptr = allocate_bytes(sizeof(T) * count, alignof(T));
        return static_cast<T*>(ptr);
    }

    template<typename T>
    void deallocate(T* ptr, std::size_t count = 1) {
        deallocate_bytes(static_cast<void*>(ptr), sizeof(T) * count, alignof(T));
    }

    void* allocate_bytes(std::size_t bytes, std::size_t alignment = alignof(std::max_align_t));
    void deallocate_bytes(void* ptr, std::size_t bytes, std::size_t alignment = alignof(std::max_align_t));
};

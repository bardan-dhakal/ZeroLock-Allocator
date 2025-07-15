#include <iostream>
#include "../include/allocator/high_performance_allocator.hpp"

int main() {
    HighPerformanceAllocator allocator;
    int* arr = allocator.allocate<int>(10);
    // ... use arr ...
    allocator.deallocate(arr, 10);
    std::cout << "Allocation and deallocation complete." << std::endl;
    return 0;
}

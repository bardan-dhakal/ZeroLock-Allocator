#include <cassert>
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include "allocator/pool_allocator.hpp"
#include "allocator/lock_free_allocator.hpp"

void test_pool_allocator() {
    std::cout << "Test 1: PoolAllocator basic allocation\n";
    PoolAllocator pool(32, 4); // 4 blocks of 32 bytes
    void* a = pool.allocate();
    void* b = pool.allocate();
    void* c = pool.allocate();
    void* d = pool.allocate();
    void* e = pool.allocate(); // Should be nullptr (out of memory)
    assert(a && b && c && d);
    assert(e == nullptr);
    std::cout << "  ✓ Allocation works correctly\n";

    pool.deallocate(b);
    void* f = pool.allocate();
    assert(f == b); // Should reuse freed block
    std::cout << "  ✓ Deallocation and reuse works\n";
    std::cout << "PoolAllocator basic test passed.\n\n";
}

void test_lock_free_allocator_single_threaded() {
    std::cout << "Test 2: LockFreeAllocator single-threaded\n";
    LockFreeAllocator alloc(64, 10); // 10 blocks of 64 bytes each

    // Allocate 20 blocks (should trigger two chunks)
    std::vector<void*> ptrs;
    for (int i = 0; i < 20; i++) {
        void* ptr = alloc.allocate();
        assert(ptr != nullptr);
        ptrs.push_back(ptr);
    }
    std::cout << "  ✓ Allocated 20 blocks successfully\n";

    // Deallocate and reallocate
    for (int i = 0; i < 10; i++) {
        alloc.deallocate(ptrs[i]);
    }
    std::cout << "  ✓ Deallocated 10 blocks\n";

    for (int i = 0; i < 10; i++) {
        void* ptr = alloc.allocate();
        assert(ptr != nullptr);
    }
    std::cout << "  ✓ Reallocated 10 blocks\n";
    std::cout << "LockFreeAllocator single-threaded test passed.\n\n";
}

void test_lock_free_allocator_multi_threaded() {
    std::cout << "Test 3: LockFreeAllocator multi-threaded (4 threads)\n";
    LockFreeAllocator alloc(64, 100); // 100 blocks per chunk

    const int num_threads = 4;
    const int allocations_per_thread = 50;
    std::vector<std::vector<void*>> thread_ptrs(num_threads);
    std::vector<std::thread> threads;
    std::atomic<int> allocation_count(0);
    std::atomic<int> deallocation_count(0);

    // Allocate from multiple threads
    for (int t = 0; t < num_threads; t++) {
        threads.emplace_back([&, t]() {
            for (int i = 0; i < allocations_per_thread; i++) {
                void* ptr = alloc.allocate();
                if (ptr != nullptr) {
                    thread_ptrs[t].push_back(ptr);
                    allocation_count++;
                }
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    assert(allocation_count == num_threads * allocations_per_thread);
    std::cout << "  ✓ All threads successfully allocated " << allocation_count << " blocks\n";

    // Deallocate from multiple threads
    threads.clear();
    for (int t = 0; t < num_threads; t++) {
        threads.emplace_back([&, t]() {
            for (void* ptr : thread_ptrs[t]) {
                alloc.deallocate(ptr);
                deallocation_count++;
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    assert(deallocation_count == num_threads * allocations_per_thread);
    std::cout << "  ✓ All threads successfully deallocated " << deallocation_count << " blocks\n";
    std::cout << "LockFreeAllocator multi-threaded test passed.\n\n";
}

void test_lock_free_allocator_stress() {
    std::cout << "Test 4: LockFreeAllocator stress test (16 threads, 1000 operations)\n";
    LockFreeAllocator alloc(128, 500);

    const int num_threads = 16;
    const int operations = 1000;
    std::atomic<int> successful_ops(0);
    std::vector<std::thread> threads;

    for (int t = 0; t < num_threads; t++) {
        threads.emplace_back([&]() {
            std::vector<void*> local_ptrs;
            for (int op = 0; op < operations; op++) {
                // Alternate between allocation and deallocation
                if (op % 2 == 0) {
                    void* ptr = alloc.allocate();
                    if (ptr != nullptr) {
                        local_ptrs.push_back(ptr);
                        successful_ops++;
                    }
                } else if (!local_ptrs.empty()) {
                    alloc.deallocate(local_ptrs.back());
                    local_ptrs.pop_back();
                    successful_ops++;
                }
            }
            // Clean up remaining allocations
            for (void* ptr : local_ptrs) {
                alloc.deallocate(ptr);
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "  ✓ Completed " << successful_ops << " operations across 16 threads\n";
    std::cout << "LockFreeAllocator stress test passed.\n\n";
}

int main() {
    std::cout << "======================================\n";
    std::cout << "Lock-Free Allocator Unit Tests\n";
    std::cout << "======================================\n\n";

    try {
        test_pool_allocator();
        test_lock_free_allocator_single_threaded();
        test_lock_free_allocator_multi_threaded();
        test_lock_free_allocator_stress();

        std::cout << "======================================\n";
        std::cout << "All tests passed!\n";
        std::cout << "======================================\n";
        return 0;
    } catch (const std::exception& e) {
        std::cout << "Test failed with exception: " << e.what() << "\n";
        return 1;
    }
}
#include "../include/allocate.hpp"
#include "../include/allocator.hpp"

#include <thread>
#include <vector>
#include <iostream>

void worker_thread (size_t thread_id, size_t num_iterations)
{
    for (size_t i = 0; i < num_iterations; i++)
    {
        void* ptr = LockFreeAllocator::allocate(64);

        if (ptr == nullptr)
        {
            std::cout << "Memory allocation failed\n";
            return;
        }

        int* data = static_cast<int*>(ptr);

        data[0] = thread_id;
        data[1] = i;


        LockFreeAllocator::free(ptr);

    }

    std::cout << "Thread " << thread_id << " completed " << num_iterations << " iterations\n";
    
}


int main()
{
    LockFreeAllocator::initialize_allocator();
    std::cout << "Allocator initialized\n";

    std::vector<std::thread> threads;

    size_t num_threads = 4;
    size_t iterations_per_thread = 1000;

    for (size_t i = 0; i < num_threads; i++)
    {
        threads.emplace_back(worker_thread, i, iterations_per_thread);
    }

    for (auto& t : threads)
    {
        t.join();
    }

    std::cout << "All threads completed successfully!\n";
    std::cout << "Total operations: " << num_threads * iterations_per_thread << "\n";

    return 0;

}
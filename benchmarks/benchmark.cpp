#include "../include/allocator.hpp"
#include "../include/allocate.hpp"
#include <thread>
#include <vector>
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <atomic>

// Global volatile to prevent optimization
volatile long global_sum = 0;

void worker_lockfree(int operations)
{
    long local_sum = 0;
    
    for (int i = 0; i < operations; i++)
    {
        void* ptr = LockFreeAllocator::allocate(64);
        if (ptr)
        {
            int* data = static_cast<int*>(ptr);
            
            // Write to multiple locations
            for (int j = 0; j < 16; j++)
            {
                data[j] = i + j;
            }
            
            // Read back and accumulate
            for (int j = 0; j < 16; j++)
            {
                local_sum += data[j];
            }
            
            // Memory barrier
            std::atomic_thread_fence(std::memory_order_seq_cst);
            
            LockFreeAllocator::free(ptr);
        }
    }
    
    // Store to global volatile
    global_sum += local_sum;
}

void worker_malloc(int operations)
{
    long local_sum = 0;
    
    for (int i = 0; i < operations; i++)
    {
        void* ptr = malloc(64);
        if (ptr)
        {
            int* data = static_cast<int*>(ptr);
            
            // Write to multiple locations
            for (int j = 0; j < 16; j++)
            {
                data[j] = i + j;
            }
            
            // Read back and accumulate
            for (int j = 0; j < 16; j++)
            {
                local_sum += data[j];
            }
            
            // Memory barrier
            std::atomic_thread_fence(std::memory_order_seq_cst);
            
            free(ptr);
        }
    }
    
    // Store to global volatile
    global_sum += local_sum;
}

double benchmark_lockfree(int num_threads, int ops_per_thread)
{
    global_sum = 0;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    std::vector<std::thread> threads;
    
    for (int i = 0; i < num_threads; i++)
    {
        threads.emplace_back(worker_lockfree, ops_per_thread);
    }
    
    for (auto& t : threads)
    {
        t.join();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    // Prevent entire benchmark from being optimized away
    if (duration.count() == 0)
    {
        return 0.0;
    }
    
    long long total_ops = static_cast<long long>(num_threads) * ops_per_thread;
    double ops_per_second = (static_cast<double>(total_ops) / duration.count()) * 1000.0;
    
    return ops_per_second;
}

double benchmark_malloc(int num_threads, int ops_per_thread)
{
    global_sum = 0;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    std::vector<std::thread> threads;
    
    for (int i = 0; i < num_threads; i++)
    {
        threads.emplace_back(worker_malloc, ops_per_thread);
    }
    
    for (auto& t : threads)
    {
        t.join();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    // Prevent entire benchmark from being optimized away
    if (duration.count() == 0)
    {
        return 0.0;
    }
    
    long long total_ops = static_cast<long long>(num_threads) * ops_per_thread;
    double ops_per_second = (static_cast<double>(total_ops) / duration.count()) * 1000.0;
    
    return ops_per_second;
}

int main()
{
    LockFreeAllocator::initialize_allocator();
    
    std::cout << "=== Memory Allocator Benchmark ===\n\n";
    
    // Use more operations to get measurable times
    int ops_per_thread = 500000;
    
    std::cout << "Testing with " << ops_per_thread << " operations per thread\n";
    std::cout << "Each operation writes and reads 16 integers\n\n";
    
    int thread_counts[] = {1, 2, 4, 8};
    
    for (int num_threads : thread_counts)
    {
        std::cout << num_threads << " Thread(s):\n";
        
        double lockfree_speed = benchmark_lockfree(num_threads, ops_per_thread);
        double malloc_speed = benchmark_malloc(num_threads, ops_per_thread);
        
        if (lockfree_speed == 0.0 || malloc_speed == 0.0)
        {
            std::cout << "  Warning: Benchmark completed too quickly, results unreliable\n\n";
            continue;
        }
        
        double speedup = lockfree_speed / malloc_speed;
        
        std::cout << "  Lock-Free: " << (lockfree_speed / 1000000.0) << "M ops/sec\n";
        std::cout << "  malloc:    " << (malloc_speed / 1000000.0) << "M ops/sec\n";
        std::cout << "  Speedup:   " << speedup << "x\n\n";
    }
    
    // Use global_sum to prevent optimization
    if (global_sum < 0)
    {
        std::cout << "Impossible sum\n";
    }
    
    return 0;
}
#include "../src/allocator/pool_allocator.cpp"
#include <cstdlib>

int main () {

    cout << "****Unit Testing for Pool Allocator****" << std::endl;

    PoolAllocator my_pool(8 , 4);

    return 0;
}
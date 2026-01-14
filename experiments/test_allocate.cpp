#include "../include/allocate.hpp"
#include "../include/allocator.hpp"
#include <iostream>

int main()
{

    initialize_allocator();

    std::cout << "Initialized and Structured Raw Memory" << std::endl;

    void* test_block = allocate(64);

    if (test_block == nullptr)
    {
        throw std::runtime_error("Allocation failed");
    }

    std::cout << "Data Allocated at - " << test_block << std::endl;

    int* data = static_cast<int*>(test_block);

    data[0] = 1234;
    data[1] = 48;

    char* char_data = static_cast<char*>(test_block) + 8;
    char_data[0] = 'A';

    bool* buul = reinterpret_cast<bool*>(static_cast<char*>(test_block) + 9);
    *buul = true;

    std::cout << "Printing data - " << data[0] << data[1] << char_data[0] << *buul << std::endl;



}
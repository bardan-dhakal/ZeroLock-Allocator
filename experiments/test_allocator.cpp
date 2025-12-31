#include <iostream>
#include "../include/allocator.hpp"


int main()
{
    initialize_allocator();

    if (free_list_head)
    {
        std::cout << "Free List Head Initialized\n" << std::endl;
    }

    size_t block_count = 0;
    while (free_list_head)
    {
        block_count++;
        
        std::cout << "Block " << block_count << " address - " << static_cast<void*>(free_list_head) << std::endl;
        std::cout << "Block Size - " << free_list_head->size << std::endl;
        std::cout << "Block Header Size - " << sizeof(*free_list_head) << std::endl;
        std::cout << "Next Block - " << static_cast<void*>(free_list_head->next) << std::endl;
        std::cout << "\n\n" ;

        free_list_head = free_list_head->next;
    }

    if (block_count != 46)
    {
        std::cout << "Block number is not 46." << std::endl;
    }

    return 0;

}
#include "../include/allocate.hpp"
#include <iostream>

void* allocate(size_t size)
{

    if (size > PAYLOAD)
    {
        return nullptr;
    }

    if (free_list_head == nullptr)
    {
        return nullptr;
    }

    else
    {
        BlockHeader* temp_block = free_list_head;

        free_list_head = free_list_head->next;

        temp_block->is_free = false;

        return reinterpret_cast<void*>(temp_block + 1);  
    }


}
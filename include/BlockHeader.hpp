#pragma once

#include <cstddef>

struct BlockHeader
{
    size_t size;
    bool is_free;
    BlockHeader* next;

    BlockHeader()
    {
        //i don't know what to do with the size_t block_size variable, would this not be chosen accordingly 
        //to the size class the users request size falls into?
        is_free = true;
        next = nullptr;
    }

};

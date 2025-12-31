#pragma once

#include <cstddef>

struct alignas(8) BlockHeader
{
    size_t size; //size of payload, 64 bytes only for now
    bool is_free; 
    BlockHeader* next;

};

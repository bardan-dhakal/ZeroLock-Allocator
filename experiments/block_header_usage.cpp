#include <iostream>
#include "../include/BlockHeader.hpp"


int main()
{
    BlockHeader block_data;

    //check for initial values
    std::cout << "Size of BlockHeader " << sizeof(BlockHeader) << " bytes" << std::endl;
    std::cout << "Alignment of BlockHeader " << alignof(BlockHeader) << " bytes" << std::endl;

    block_data.size = 64;
    block_data.is_free = true;
    block_data.next = nullptr;

    std::cout << "After initialization" << std::endl;
    std::cout << "Block size - " << block_data.size << std::endl;
    std::cout << "Is Free? " << block_data.is_free << std::endl;
    std::cout << "Next free block - " << block_data.next << std::endl;
    
    std::cout << "Pointer Arithmetic" << std::endl;
    std::cout << "Block Header Address - " << &block_data << std::endl;
    std::cout << "Payload Address - " << static_cast<void*>(&block_data + 1) << std::endl;
    std::cout << "Block Header to Payload offset " << reinterpret_cast<char*>(&block_data + 1) - reinterpret_cast<char*>(&block_data) << " bytes" << std::endl;

    return 0;
}

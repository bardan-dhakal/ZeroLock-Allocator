#include <iostream>
#include <sys/mman.h>
#include <cstring>

void* allocate_memory(size_t size)
{
    void* mem_addr;
    
    int permissions = PROT_READ | PROT_WRITE; //memory protection - allows for read and write

    int flag = MAP_ANONYMOUS | MAP_PRIVATE; //mapping is not backed by any file, memory is raw

    int file_descriptor = -1; //no file required

    off_t offset = 0; //no required offset

    mem_addr = mmap(NULL, size, permissions, flag, file_descriptor, offset);

    if  (mem_addr == MAP_FAILED)
    {
        throw std::runtime_error("Memory allocation unsuccessful");
    }

    return mem_addr;

}



int main()
{
    void* ptr;

    size_t required_bytes = 4096; //how many bytes you want

    try
    {
        ptr = allocate_memory(required_bytes);
        std::cout << "Memory allocated at " << ptr << std::endl;
    }

    catch (const std::runtime_error& e)
    {
        std::cerr << "Error" << e.what() << std::endl;
        return 1;
    }

    char* write_ptr = static_cast<char*>(ptr);
    std:: string write_data = "I wrote data on to the allocated memory. I wrote data on to the allocated memory. I wrote data on to the allocated memory. I wrote data on to the allocated memory.";
    size_t totalBytes = write_data.length();

    if (totalBytes > required_bytes)
    {
        std::cout << "Write data too large" << std::endl;
        return 1;
    }

    
    memcpy(write_ptr, write_data.c_str(), totalBytes);


    std::string read_data(write_ptr);
    std::cout << "Read data is - " << read_data << std::endl;

    munmap(ptr, required_bytes);



    return 0;
}
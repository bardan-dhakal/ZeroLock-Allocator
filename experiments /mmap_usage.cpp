#include <iostream>
#include <sys/mman.h>

void* make_system_call(void * length, size_t size, int prot, int flag, int fd, off_t offset)
{
    void* mem_addr;

    mem_addr = mmap(length, size, prot, flag, fd, offset);

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

    int permissions = PROT_READ | PROT_WRITE; //memory protection - allows for read and write

    int flag = MAP_ANONYMOUS | MAP_SHARED; //mapping is not backed by any file, memory is raw

    int file_descriptor = -1; //no file required

    off_t offset = 0; //no required offset


    try
    {
        ptr = make_system_call(NULL, required_bytes, permissions, flag, file_descriptor, offset);
        std::cout << "Memory allocated at " << ptr << std::endl;
    }

    catch (const std::runtime_error& e)
    {
        std::cerr << "Error" << e.what() << std::endl;
        return 1;
    }

    char* write_ptr = static_cast<char*>(ptr);
    char write_data[] = "I wrote data on to the allocated memory";

    write_ptr = write_data;


    std::string read_data(write_ptr);
    std::cout << "Read data is - " << read_data << std::endl;



    return 0;
}
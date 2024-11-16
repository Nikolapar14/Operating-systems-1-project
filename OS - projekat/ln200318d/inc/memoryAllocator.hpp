

#ifndef PROJEKAT1_1_MEMORYALLOCATOR_HPP
#define PROJEKAT1_1_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

struct FreeMem{
    FreeMem* next;
    FreeMem* prev;
    size_t size;
};

class MemoryAllocator{
public:

    void setSizeInBlocks(size_t sz){
        sizeInBlockss = sz;
        return;
    }

    size_t getSizeInBlocks(){
        return sizeInBlockss;
    }

     void* memory_alloc(size_t size);

     int memory_free(void* vd);

     int tryToJoin(FreeMem* cur);

     static MemoryAllocator* getMemoryAllocator();

    static FreeMem* fmem_head;

    size_t sizeInBlockss;

private:

    static MemoryAllocator* singleton;

};

#endif //PROJEKAT1_1_MEMORYALLOCATOR_HPP

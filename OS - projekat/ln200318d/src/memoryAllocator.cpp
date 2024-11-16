
#include "../h/memoryAllocator.hpp"


FreeMem* MemoryAllocator::fmem_head = nullptr;
MemoryAllocator* MemoryAllocator::singleton = nullptr;

MemoryAllocator *MemoryAllocator::getMemoryAllocator() {
    if(MemoryAllocator::singleton!=nullptr){
        return MemoryAllocator::singleton;
    }else{
        singleton = (MemoryAllocator*)HEAP_START_ADDR;
        size_t sizeInBlocks = sizeof(MemoryAllocator)/MEM_BLOCK_SIZE;

        if((sizeof(MemoryAllocator)%MEM_BLOCK_SIZE) != 0){
            sizeInBlocks++;
        }
        singleton->sizeInBlockss =sizeInBlocks;
        FreeMem* head = (FreeMem*)((uint64*)HEAP_START_ADDR + singleton->sizeInBlockss*MEM_BLOCK_SIZE);

        size_t heapBlocks = ((uint64*)HEAP_END_ADDR - (uint64*)HEAP_START_ADDR)/MEM_BLOCK_SIZE;

        size_t freeBlocks = heapBlocks - singleton->sizeInBlockss;

        head->size = freeBlocks;
        head->next = nullptr;
        head->prev = nullptr;
        fmem_head = head;

        return singleton;
    }
}

void* MemoryAllocator::memory_alloc(size_t size){
   for(FreeMem* cur = MemoryAllocator::fmem_head; cur != nullptr; cur = cur->next){
       if(cur->size < size)continue;

       if(cur->size == size){
           if(cur->prev) cur->prev->next = cur->next;
           else MemoryAllocator::fmem_head = cur->next;
           cur->next=nullptr;
           cur->prev=nullptr;
           return (void*)((uint64 *)cur + sizeof(FreeMem));
       }
       else{
           FreeMem* newFrag = (FreeMem*)((uint64*)cur + size*MEM_BLOCK_SIZE);
           if(cur->prev)cur->prev->next = newFrag;
           else MemoryAllocator::fmem_head = newFrag;
           if(cur->next)cur->next->prev= newFrag;

           newFrag->prev = cur->prev;
           newFrag->next = cur->next;
           newFrag->size = cur->size - size;
           cur->next=nullptr;
           cur->prev=nullptr;
           cur->size = size;
           return (void*)((uint64*)cur + sizeof(FreeMem));
       }

   }

    return nullptr;

};

int MemoryAllocator::memory_free(void* vd){

    FreeMem* cur = nullptr;

    if(!fmem_head || vd < (uint64*)fmem_head)cur=nullptr;
    else{
        for(cur =fmem_head; cur->next != 0 && vd > (uint64*)(cur->next);cur=cur->next);
    }

    FreeMem* old = (FreeMem*)((uint64*)vd - sizeof(FreeMem));
    if(cur) old->next = cur->next;
    else old->next =fmem_head;
    if(old->next)old->next->prev = old;
    if(cur) cur->next = old;
    else fmem_head = old;

    tryToJoin(old);
    tryToJoin(cur);

    return 0;

}

int MemoryAllocator::tryToJoin(FreeMem *cur) {
    if(!cur)return 0;
    if(cur->next && ((uint64*)cur + cur->size*MEM_BLOCK_SIZE) == (uint64*)cur->next){
        cur->size += cur->next->size;
        cur->next = cur->next->next;
        if(cur->next)cur->next->prev = cur;
        return 1;
    }else{
        return 0;
    }
};

#include "../h/scheduler.hpp"

StaticList<TCB>* Scheduler::readyThreadQueue;
bool Scheduler::napravljen = false;

TCB *Scheduler::get()
{
    if(!napravljen){
        Scheduler::inicijalizuj();
        Scheduler::napravljen = true;
    }
    TCB* ret = readyThreadQueue->removeFirst();
    return ret;
}

void Scheduler::put(TCB *ccb)
{
    if(!napravljen){
        Scheduler::inicijalizuj();
        Scheduler::napravljen = true;
    }
    readyThreadQueue->addLast(ccb);
}

void Scheduler::inicijalizuj() {

    MemoryAllocator* singleton = MemoryAllocator::getMemoryAllocator();
    Scheduler::readyThreadQueue = (StaticList<TCB>*)singleton->memory_alloc(sizeof(StaticList<TCB>));
    readyThreadQueue->inicijalizuj();

}

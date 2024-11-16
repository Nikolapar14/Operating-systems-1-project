
#include "../h/Sem.hpp"
#include "../h/syscall_c.hpp"


uint64 Sem::semIdStatic = 0;


void inline Sem::block() {
    TCB* old = TCB::running;
    blockedThreadQueue->addLast(old);


    TCB::running = Scheduler::get();
    TCB::contextSwitch(&old->context, &TCB::running->context);
}

void inline Sem::unblock() {

    TCB* unblocked = blockedThreadQueue->removeFirst();
    Scheduler::put(unblocked);

}

int Sem::waitSem(Sem *handle) {

    if(--handle->val<0)handle->block();

    return 0;
}

int Sem::signalSem(Sem *handle) {

    if(++handle->val<=0)handle->unblock();

    return 0;
}

int Sem::createSem(Sem** handle, int init) {

    MemoryAllocator* singleton = MemoryAllocator::getMemoryAllocator();
    Sem* semaphore = (Sem*)singleton->memory_alloc(sizeof(Sem));
    initializeSem(semaphore,init);
    *handle = semaphore;
    return 0;

}

int Sem::closeSem(Sem *handle) {
    TCB* first = handle->blockedThreadQueue->removeFirst();
    while(first){
        Scheduler::put(first);
        first = handle->blockedThreadQueue->removeFirst();
    }
    return 0;
}

void Sem::initializeSem(Sem *semaphore, int init) {

    semaphore->val = init;
    semaphore->semId = semIdStatic++;
    MemoryAllocator* singleton = MemoryAllocator::getMemoryAllocator();
    semaphore->blockedThreadQueue = (StaticList<TCB>*)singleton->memory_alloc(sizeof(StaticList<TCB>));
    semaphore->blockedThreadQueue->inicijalizuj();


}





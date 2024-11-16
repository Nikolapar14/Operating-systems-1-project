
#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../test/printing.hpp"


TCB *TCB::running = nullptr;



int TCB::createThread(TCB** thread,void* args,Body body, uint64* stackPtr, bool type)
{
    MemoryAllocator* singleton = MemoryAllocator::getMemoryAllocator();

    TCB* newTCB = (TCB*)singleton->memory_alloc(sizeof(TCB));
    initialize(newTCB,args,body,stackPtr);
    if(body!= nullptr && !type)Scheduler::put(newTCB);
    *thread = newTCB;
    return 0;
}

void TCB::yield()
{
    __asm__ volatile ("ecall");
}

void TCB::dispatch()
{
    TCB *old = running;
    if (!old->isFinished()) { Scheduler::put(old); }
    else delete(old);

    running = Scheduler::get();

    TCB::contextSwitch(&old->context, &running->context);

}

void TCB::threadWrapper()
{
    Riscv::popSppSpie();
    running->body(running->args);
    running->setFinished(true);
    printString("thread finished");

    thread_dispatch();

}

void TCB::join(TCB* handle) {
    while(!handle->isFinished()){
        TCB::dispatch();
    }
}



void TCB::initialize(TCB *thread, void *args, TCB::Body body, uint64 *stackPtr) {
    thread->body = body;
    thread->args = args;
    if(body != nullptr){
        thread->stack = stackPtr;
    }else{
        thread->stack = nullptr;
    }
    thread->context.ra = (uint64)&TCB::threadWrapper;
    if(thread->stack != nullptr){
        thread->context.sp = (uint64) &stackPtr[DEFAULT_STACK_SIZE];
    }else{
        thread->context.sp = 0;
    }
    thread->finished = false;
    thread->next = nullptr;
}





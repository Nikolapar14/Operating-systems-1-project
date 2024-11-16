
#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP

#include "../lib/hw.h"
#include "scheduler.hpp"
#include "../h/memoryAllocator.hpp"

class TCB
{
public:

    void* operator new(size_t size){
        uint64 volatile blocks;

        if(size + sizeof(FreeMem) < MEM_BLOCK_SIZE){
            blocks=1;
        }else{
            blocks = (size + sizeof(FreeMem)) / MEM_BLOCK_SIZE;
            if((size + sizeof(FreeMem))%MEM_BLOCK_SIZE != 0){
                blocks++;
            }
        }
        return MemoryAllocator::getMemoryAllocator()->memory_alloc(blocks);
    }

    void operator delete(void *p) noexcept{
        MemoryAllocator::getMemoryAllocator()->memory_free(p);
    }

    ~TCB() { delete[] stack; }

    bool isFinished() const { return finished; }

    void setFinished(bool value) { finished = value; }

    using Body = void (*)(void*);

    static void initialize(TCB* thread,void* args,Body body, uint64* stackPtr);

    static int createThread(TCB** thread,void* args,Body body, uint64* stackPtr, bool type);

    static void yield();

    static void join(TCB* handle);

    static TCB *running;

private:
    TCB(void* args,Body body, uint64* stackPtr, bool type) :

            body(body),
            args(args),
            stack(body != nullptr ? stackPtr : nullptr),
            context({(uint64) &threadWrapper,
                     stack != nullptr ? (uint64) &stackPtr[DEFAULT_STACK_SIZE] : 0
                    }),

            finished(false)

    {
        if (body != nullptr && !type) { Scheduler::put(this); }
    }

    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    TCB* next;
    Body body;
    void* args;
    uint64 *stack;
    Context context;

    bool finished;

    friend class Riscv;
    friend class Sem;
    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void dispatch();

    static uint64 constexpr STACK_SIZE = 1024;
    static uint64 constexpr TIME_SLICE = 2;
};

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP

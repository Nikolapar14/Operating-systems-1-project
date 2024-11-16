

#ifndef PROJEKAT1_1_SEM_HPP
#define PROJEKAT1_1_SEM_HPP

#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../h/staticList2.hpp"

#include "memoryAllocator.hpp"

extern "C" uint64 copy_and_swap(uint64 &lock, uint64 expected, uint64 desired);

class Sem{

public:

    friend class Riscv;

    static int waitSem(Sem* handle);
    static int signalSem(Sem* handle);
    static int createSem(Sem** handle, int init);
    static int closeSem(Sem* handle);
    static void initializeSem(Sem* semaphore,int init);

private:

    Sem(uint64 init):val(init){
        semId = semIdStatic++;
    }
     StaticList<TCB>* blockedThreadQueue;

    void inline block();
    void inline unblock();

    int val;
    static uint64 semIdStatic;
    uint64 semId;

};

#endif //PROJEKAT1_1_SEM_HPP

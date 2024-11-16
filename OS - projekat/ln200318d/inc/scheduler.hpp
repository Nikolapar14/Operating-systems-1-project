
#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_SCHEDULER_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_SCHEDULER_HPP


#include "../h/staticList2.hpp"

class TCB;

class Scheduler
{
private:
    static StaticList<TCB>* readyThreadQueue;
    static bool napravljen;

public:
    static TCB *get();

    static void put(TCB *ccb);

    static void inicijalizuj();

};

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_SCHEDULER_HPP

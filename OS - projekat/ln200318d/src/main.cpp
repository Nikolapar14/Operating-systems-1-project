

#include "../lib/console.h"
#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../lib/hw.h"
#include "../h/memoryAllocator.hpp"
#include "../h/syscall_c.hpp"
#include "../test/userMain.hpp"
#include "../test/printing.hpp"


int main()
{
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);


    thread_t mainThread;
    thread_t userMainWrapper;
    thread_create(&mainThread,nullptr,nullptr);
    thread_create(&userMainWrapper,userMain,nullptr);

    TCB::running = mainThread;
    thread_join(userMainWrapper);

    return 0;

}

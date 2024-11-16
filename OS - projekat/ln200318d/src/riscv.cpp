
#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../lib/console.h"
#include "../h/memoryAllocator.hpp"
#include "../test/printing.hpp"
#include "../h/Sem.hpp"

void Riscv::popSppSpie()
{
    mc_sstatus(SSTATUS_SPP);
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::handleSupervisorTrap()
{
    uint64 x0,x1,x2,x3,x4,x5;

    __asm__ volatile("mv %[x], a0" : [x] "=r" (x0));
    __asm__ volatile("mv %[x], a1" : [x] "=r" (x1));
    __asm__ volatile("mv %[x], a2" : [x] "=r" (x2));
    __asm__ volatile("mv %[x], a3" : [x] "=r" (x3));
    __asm__ volatile("mv %[x], a4" : [x] "=r" (x4));
    __asm__ volatile("mv %[x], a5" : [x] "=r" (x5));

    uint64 scause = r_scause();

    uint64 x = x0;
    //__asm__ volatile("mv %[x], a0" : [x] "=r" (x));

    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL)
    {
        if(x==0x01){
            //mem_alloc
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();
            size_t size= (size_t)x1;
            //__asm__ volatile("mv %[size], a1" : [size] "=r" (size));
            MemoryAllocator* instance = MemoryAllocator::getMemoryAllocator();
            void* returnAddress = instance->memory_alloc(size);
            __asm__ volatile ("mv a0, %[ret]" : : [ret] "r" (returnAddress));

            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        else if(x==0x02){
            //mem_free
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();
            void* address = (void*)x1;
            //__asm__ volatile("mv %[size], a1" : [size] "=r" (address));
            MemoryAllocator* instance = MemoryAllocator::getMemoryAllocator();
            int ret = instance->memory_free(address);
            __asm__ volatile ("mv a0, %[ret]" : : [ret] "r" (ret));

            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        else if(x==0x11){
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();

            using Body = void (*)(void*);

            void* stackPtr = (void*)x4;
            Body b = (TCB::Body) x2 ;
            void* arg = (void*)x3;
            TCB** handle = (TCB**)x1;
            uint64* newStack = (uint64*)stackPtr;

            int i = TCB::createThread(handle,arg,b,newStack,false);
            __asm__ volatile ("mv a0, %[ret]" : : [ret] "r" (i));

            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        else if(x == 0x12){
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();

            TCB::running->setFinished(true);
            TCB::dispatch();

            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        else if(x == 0x13){
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();

            TCB::dispatch();

            w_sstatus(sstatus);
            w_sepc(sepc);

        }
        else if(x == 0x14){
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();

            TCB* handle = (TCB*)x1;
            TCB::join(handle);

            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        else if(x ==0x15){
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();

            using Body = void (*)(void*);


            void* stackPtr = (void*)x4;
            Body b = (TCB::Body) x2 ;
            void* arg = (void*)x3;
            TCB** handle = (TCB**)x1;

            int i = TCB::createThread(handle,arg,b,(uint64*)stackPtr,true);
            __asm__ volatile ("mv a0, %[ret]" : : [ret] "r" (i));

            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        else if(x == 0x21){
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();

            int init = (int)x2;
            sem_t* handle = (sem_t*)x1;

            int i = Sem::createSem(handle,init);
            __asm__ volatile ("mv a0, %[ret]" : : [ret] "r" (i));

            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        else if(x == 0x22){
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();

            sem_t handle = (sem_t)x1;

            int i = Sem::closeSem(handle);
            __asm__ volatile ("mv a0, %[ret]" : : [ret] "r" (i));

            w_sstatus(sstatus);
            w_sepc(sepc);

        }
        else if(x == 0x23){
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();

            sem_t handle = (sem_t)x1;

            int i = Sem::waitSem(handle);
            __asm__ volatile ("mv a0, %[ret]" : : [ret] "r" (i));

            w_sstatus(sstatus);
            w_sepc(sepc);

        }
        else if(x == 0x24){
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();

            sem_t handle = (sem_t)x1;

            int i = Sem::signalSem(handle);
            __asm__ volatile ("mv a0, %[ret]" : : [ret] "r" (i));

            w_sstatus(sstatus);
            w_sepc(sepc);

        }
        else if(x ==0x41){
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();
            char ch = __getc();
            __asm__ volatile ("mv a0, %[ret]" : : [ret] "r" ((uint64)ch));
            w_sstatus(sstatus);
            w_sepc(sepc);

        }
        else if(x == 0x42){
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();

            char ch = (char)x1;
            //__asm__ volatile("mv %[ch], a1" : [ch] "=r" (ch));
            __putc(ch);

            w_sstatus(sstatus);
            w_sepc(sepc);
        }

    }
    else if (scause == 0x8000000000000001UL)
    {

        mc_sip(SIP_SSIP);

    }
    else if (scause == 0x8000000000000009UL)
    {

        console_handler();
    }
    else if(scause == 0x000000000000002UL)
    {

        printString("greska");
        for(int count =0;count<1000000;count++);
    }else{

        printString("greska");

    }
}
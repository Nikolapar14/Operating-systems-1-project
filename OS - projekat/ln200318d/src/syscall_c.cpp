
#include "../h/syscall_c.hpp"


void* mem_alloc(size_t size){

    uint64 volatile code = 0x01;
    uint64 volatile blocks;

    if(size + sizeof(FreeMem) < MEM_BLOCK_SIZE){
        blocks=1;
    }else{
        blocks = (size + sizeof(FreeMem)) / MEM_BLOCK_SIZE;
        if((size + sizeof(FreeMem))%MEM_BLOCK_SIZE != 0){
            blocks++;
        }
    }
    __asm__ volatile ("mv a1, %[blocks]" : : [blocks] "r" (blocks));
    __asm__ volatile ("mv a0, %[code]" : : [code] "r" (code));
    __asm__ volatile ("ecall");

    uint64 volatile returnAddress;

    __asm__ volatile("mv %[ret], a0" : [ret] "=r" (returnAddress));

    return (void*)returnAddress;

}

int mem_free(void* vd){

    uint64 volatile code = 0x02;

    __asm__ volatile ("mv a1, %[blocks]" : : [blocks] "r" (vd));
    __asm__ volatile ("mv a0, %[code]" : : [code] "r" (code));
    __asm__ volatile ("ecall");

    int ret;
    __asm__ volatile("mv %[ret], a0" : [ret] "=r" (ret));

    return ret;

}

int thread_create(TCB** handle, void(*start_routine)(void*),void* arg){


    void* volatile stack = new uint64[DEFAULT_STACK_SIZE];
    void* args = arg;
    TCB** han = handle;
    void(*start)(void*) = start_routine;

    __asm__ volatile ("mv a4, %[stack]" : : [stack] "r" (stack));
    __asm__ volatile ("mv a3, %[argument]" : : [argument] "r" (args));
    __asm__ volatile ("mv a2, %[start]" : : [start] "r" (start));
    __asm__ volatile ("mv a1, %[handle]" : : [handle] "r" (han));
    __asm__ volatile ("li a0, 0x11");
    __asm__ volatile ("ecall");

    int ret;
    __asm__ volatile("mv %[ret], a0" : [ret] "=r" (ret));

    return ret;
}

int thread_exit(){
    __asm__ volatile ("li a0, 0x12");
    __asm__ volatile ("ecall");

    int ret;
    __asm__ volatile("mv %[ret], a0" : [ret] "=r" (ret));

    return 0;

}

void thread_dispatch(){
    __asm__ volatile ("li a0, 0x13");
    __asm__ volatile ("ecall");

    return;
}

void thread_join(thread_t handle){


    thread_t han = handle;

    __asm__ volatile ("mv a1, %[handle]" : : [handle] "r" (han));
    __asm__ volatile ("li a0, 0x14");
    __asm__ volatile("ecall");

    return;
}

int thread_createFromCPPApi(TCB** handle, void(*start_routine)(void*),void* arg){



    void* volatile stack = new uint64[DEFAULT_STACK_SIZE];
    void* args = arg;
    TCB** han = handle;
    void(*start)(void*) = start_routine;



    __asm__ volatile ("mv a4, %[stack]" : : [stack] "r" (stack));
    __asm__ volatile ("mv a3, %[argument]" : : [argument] "r" (args));
    __asm__ volatile ("mv a2, %[start]" : : [start] "r" (start));
    __asm__ volatile ("mv a1, %[handle]" : : [handle] "r" (han));
    __asm__ volatile ("li a0, 0x15");
    __asm__ volatile ("ecall");

    int ret;
    __asm__ volatile("mv %[ret], a0" : [ret] "=r" (ret));

    return ret;
}

int sem_open(sem_t* handle, unsigned init){
    uint64 code = 0x21;
    sem_t* han = handle;
    unsigned i = init;

    __asm__ volatile ("mv a2, %[i]" : : [i] "r" (i));
    __asm__ volatile ("mv a1, %[handle]" : : [handle] "r" (han));
    __asm__ volatile ("mv a0, %[code]" : : [code] "r" (code));
    __asm__ volatile ("ecall");

    int ret;
    __asm__ volatile("mv %[ret], a0" : [ret] "=r" (ret));

    return ret;

}

int sem_close(sem_t handle){
    uint64 code = 0x22;
    sem_t han = handle;

    __asm__ volatile ("mv a1, %[handle]" : : [handle] "r" (han));
    __asm__ volatile ("mv a0, %[code]" : : [code] "r" (code));
    __asm__ volatile ("ecall");

    int ret;
    __asm__ volatile("mv %[ret], a0" : [ret] "=r" (ret));

    return ret;
}

int sem_wait(sem_t handle){
    uint64 code = 0x23;
    sem_t han = handle;

    __asm__ volatile ("mv a1, %[handle]" : : [handle] "r" (han));
    __asm__ volatile ("mv a0, %[code]" : : [code] "r" (code));
    __asm__ volatile ("ecall");

    int ret;
    __asm__ volatile("mv %[ret], a0" : [ret] "=r" (ret));

    return ret;
}

int sem_signal(sem_t handle){
    uint64 code = 0x24;
    sem_t han = handle;

    __asm__ volatile ("mv a1, %[handle]" : : [handle] "r" (han));
    __asm__ volatile ("mv a0, %[code]" : : [code] "r" (code));
    __asm__ volatile ("ecall");

    int ret;
    __asm__ volatile("mv %[ret], a0" : [ret] "=r" (ret));

    return ret;
}

char getc(){
    uint64 chRet;
    uint64 code = 0x41;
    __asm__ volatile ("mv a0, %[code]" : : [code] "r" (code));
    //__asm__ volatile("mv %[ret], a0" : [ret] "=r" (chRet));
    __asm__ volatile("ecall");
    __asm__ volatile("mv %[ret], a0" : [ret] "=r" (chRet));
    return (char)chRet;
}

void putc(char ch){
    uint64 chArg = ch;
    uint64 code = 0x42;

    __asm__ volatile ("mv a1, %[ch]" : : [ch] "r" (chArg));
    __asm__ volatile ("mv a0, %[code]" : : [code] "r" (code));
    __asm__ volatile("ecall");

    return;
}
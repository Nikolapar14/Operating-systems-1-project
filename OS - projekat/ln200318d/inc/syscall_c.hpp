

#ifndef PROJEKAT1_1_SYSCALL_C_HPP
#define PROJEKAT1_1_SYSCALL_C_HPP

#include "../lib/hw.h"
#include "../h/tcb.hpp"
#include "../h/Sem.hpp"
#include "../h/memoryAllocator.hpp"

void* mem_alloc(size_t size);

int mem_free(void*);

typedef TCB* thread_t;

int thread_create(TCB** handle, void(*start_routine)(void*),void* arg);

int thread_exit();

void thread_dispatch();

void thread_join(thread_t handle);

int thread_createFromCPPApi(TCB** handle, void(*start_routine)(void*),void* arg);

typedef Sem* sem_t;

int sem_open(sem_t* handle,unsigned init);

int sem_close(sem_t handle);

int sem_wait(sem_t handle);

int sem_signal(sem_t id);

char getc();

void putc(char c);

#endif //PROJEKAT1_1_SYSCALL_C_HPP

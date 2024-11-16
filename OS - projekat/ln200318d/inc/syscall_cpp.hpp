
#ifndef PROJEKAT1_1_SYSCALL_CPP_HPP
#define PROJEKAT1_1_SYSCALL_CPP_HPP

#include "syscall_c.hpp"
#include "../h/scheduler.hpp"
#include "../lib/console.h"



void* operator new(size_t n);
void* operator new[](size_t n);

void operator delete(void *p) noexcept;
void operator delete[](void *p) noexcept;

class Thread{
public:
    Thread(void (*body)(void*), void* arg);
    virtual ~Thread();

    int start();

    void join();

    static void dispatch();
    //static int sleep (time_t);


    static void runWrapper(void*);

protected:
    Thread();

    virtual void run(){}

private:

    TCB* myHandle;
    void (*body)(void*);
    void* arg;

};

class Semaphore{
public:
    Semaphore(unsigned init = 1);
    virtual ~Semaphore();

    int wait();
    int signal();

private:
    sem_t myHandle;
};

class Console{
public:
    static char getc();
    static void putc(char c);
};

#endif //PROJEKAT1_1_SYSCALL_CPP_HPP

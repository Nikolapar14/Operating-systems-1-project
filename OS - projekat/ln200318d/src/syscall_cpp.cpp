
#include "../h/syscall_cpp.hpp"

void* operator new(size_t n){
    return mem_alloc(n);
}

void* operator new[](size_t n){
    return mem_alloc(n);
}

void operator delete(void *p) noexcept{
    mem_free(p);

}

void operator delete[](void *p) noexcept{
    mem_free(p);
    return;
}

Thread::Thread(void (*body)(void *), void *arg) {
    thread_createFromCPPApi(&myHandle, body,arg);
}

void Thread::join() {
    if(myHandle){
        thread_join(myHandle);
    }
}

Thread::~Thread() {
    myHandle->setFinished(true);
     //delete myHandle;
}

int Thread::start() {
    Scheduler::put(myHandle);
    return 0;
}

void Thread::dispatch() {
    thread_dispatch();
}

Thread::Thread() {
    thread_createFromCPPApi(&myHandle,runWrapper,this);
}

void Thread::runWrapper(void* arg) {

    Thread*t = (Thread*)arg;
    t->run();

}

Semaphore::Semaphore(unsigned int init) {

    sem_open(&myHandle,init);

}

Semaphore::~Semaphore() {

    sem_close(myHandle);

}

int Semaphore::wait() {
   return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}

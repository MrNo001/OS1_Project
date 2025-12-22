//
// Created by os on 3/2/25.
//

#include "../h/syscall_cpp.hpp"
#include "../h/KConsole.hpp"

//-------------MEMORY--------------------------
void* operator new(size_t size){
    return mem_alloc(size);
}

void operator delete(void* mem){
    mem_free(mem);
}



//---------------THREAD------------------------


Thread::Thread(void (*body)(void*), void* args){
    myHandle = nullptr;
    this->body = body;
    this->arg = args;
    thread_build(&myHandle,body,args);

}

Thread::~Thread(){
    thread_delete(myHandle);
}


int Thread::start() {
   return thread_start(myHandle);
}

void Thread::dispatch() {
     thread_dispatch();
}

int Thread::sleep(time_t interval) {
    return time_sleep(interval);
}

Thread::Thread() {
    myHandle = nullptr;
    thread_build(&myHandle, &Thread::wrapper, (void*)this);
}

void Thread::wrapper(void* thread) {
    ((Thread*)thread)->run();
}


//---------------SEMAPHORE------------------------

Semaphore::Semaphore(unsigned int init) {
    int res = sem_open(&myHandle, init);
    if(res == -1) {
        KConsole::kprintln("Error: sem_open failed");
        return;
    }
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::wait() {
    if(!myHandle) return -1;
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    if(!myHandle) return -1;
    return sem_signal(myHandle);
}


//---------------PERIODIC THREAD------------------------

struct PTargs {
    PeriodicThread* thread;
    time_t period;
};

PeriodicThread::PeriodicThread(time_t period)
        : Thread(&PeriodicThread::wrapper, new PTargs({this, period})),
          period(period), terminated(false) {}

void PeriodicThread::wrapper(void* args) {
    PeriodicThread* thread = (PeriodicThread*)((PTargs*)args)->thread;
    time_t period = ((PTargs*)args)->period;

    while (!thread->terminated) {
        thread->periodicActivation();
        Thread::sleep(period);
    }


}

void PeriodicThread::terminate() {
    terminated = true;

}

//---------------CONSOLE------------------------

char Console::getc() {
    return ::getc();
}
void Console::putc(char c) {
    ::putc(c);
}
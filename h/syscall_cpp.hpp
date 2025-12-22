//
// Created by os on 3/2/25.
//
#ifndef _syscall_cpp
#define _syscall_cpp

#include "syscall_c.hpp"


//-----------------MEMORY----------------------
void* operator new (size_t);
void operator delete (void*);


//-----------------THREAD----------------------
class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();

    int start ();

    static void dispatch ();
    static int sleep (time_t);

protected:
    Thread ();
    virtual void run () {}

private:
    thread_t myHandle;
    void (*body)(void*); void* arg;
    static void wrapper(void* args);
};


//-----------------SEMAPHORE----------------------
class Semaphore {
public:
    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();
    int wait ();
    int signal ();
private:
    sem_t myHandle;
};


//-----------------PERIODIC THREAD----------------------
class PeriodicThread : public Thread {
public:
    void terminate ();
protected:
    PeriodicThread (time_t period);
    virtual void periodicActivation () {}
private:
    time_t period;
    static void wrapper(void* arg);
    bool terminated;
};


//-----------------CONSOLE----------------------
class Console {
public:
    static char getc ();
    static void putc (char);
};


#endif
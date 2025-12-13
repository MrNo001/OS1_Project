//
// Created by os on 1/29/25.
//

#ifndef OS_PROJECT_TCB_HPP
#define OS_PROJECT_TCB_HPP

#include "../lib/hw.h"
#include "../h/riscv.hpp"
#include "../h/Kernel.hpp"
#include "../h/KSemaphore.hpp"

class TCB{

public:

    static int counter_id;

    int thread_id;

    using Body = void (*)(void *);

    ~TCB(){ if(stack != nullptr) MemoryAllocator::kfree(stack);}

    TCB(Body body , void* args,uint64* stack = (uint64*)MemoryAllocator::kmalloc(DEFAULT_STACK_SIZE) , uint64 timeSlice=DEFAULT_TIME_SLICE);

    bool isFinished() const {return finished;}

    void setFinished(bool value) { finished = value; }

    uint64 getTimeSlice() const { return timeSlice; }






private:

    static uint64 timeSliceCounter;

    static TCB* running;

    struct Context{
        uint64 pc;
        uint64 sp;
    };

    Body body;
    void* args;
    uint64* stack;
    Context context;
    uint64 timeSlice;
    bool finished;

    uint64 wake_time;

    uint64 saved_sp;


    static void contextSwitch(Context *oldContext, Context *newContext);

    static void dispatch();

    static void stop();

    void start();

    static void ThreadWrapper();

    static void threadCreateSCHandler();
    static void threadExitSCHandler();
    static void threadDispatchSCHandler();
    static void threadSleepSCHandler();
    static void threadDeleteSCHandler();
    static void threadBuildSCHandler();

public:
    void* operator new(size_t size);
    void operator delete(void* addr);

    friend class Riscv;
    friend class Kernel;
    friend class KSemaphore;
    friend class KConsole;
    friend class Scheduler;

};


#endif //OS_PROJECT_TCB_HPP

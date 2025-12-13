//
// Created by os on 6/30/25.
//

#ifndef OS_PROJECT_KSEMAPHORE_HPP
#define OS_PROJECT_KSEMAPHORE_HPP


#include "../h/Queue.hpp"
#include "../h/TCB.hpp"


class TCB;

class KSemaphore {
public:
    KSemaphore(unsigned init);
    static void semOpenSCHandler();
    static void semCloseSCHandler();
    static void semWaitSCHandler();
    static void semSignalSCHandler();
    int counter;


    Queue<TCB*> waitingQ;

    int wait();
    int signal();


    void operator delete(void* ptr);
    void* operator new(size_t size);

};


#endif //OS_PROJECT_KSEMAPHORE_HPP

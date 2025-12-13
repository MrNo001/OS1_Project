//
// Created by os on 6/29/25.
#include "../h/KSemaphore.hpp"
#include "../h/Scheduler.hpp"
#include "../h/MemAllocator.hpp"
#include "../lib/hw.h"
#include "../h/riscv.hpp"
#include "../h/Queue.hpp"
#include "../h/TCB.hpp"



void KSemaphore::operator delete(void *ptr)  {
    MemoryAllocator::kfree(ptr);
    return;
}

void* KSemaphore::operator new(size_t size) {
    return MemoryAllocator::kmalloc(size);
}

KSemaphore::KSemaphore(unsigned init) {
    this->counter = init;
}



int KSemaphore::wait(){
    if(this->counter == 0){
        waitingQ.put(TCB::running);
        TCB::stop();
    }
    else{
        this->counter--;
    }
    return 1;
}

int KSemaphore::signal() {
    if(waitingQ.isEmpty()){
        counter++;
    }
    else{
        TCB* to_put = waitingQ.pop();
        Scheduler::put(to_put);
    }
    return 1;
}

void KSemaphore::semOpenSCHandler(){

    uint64 init;
    KSemaphore** handle;

    __asm__ volatile("mv %0, a2" : "=r"(init));
    __asm__ volatile("mv %0, a1" : "=r"(handle));

    KSemaphore* newKSemaphore = new KSemaphore(init);
    (*handle) = newKSemaphore;

    Riscv::w_a0(0);

}

void KSemaphore::semCloseSCHandler() {
    KSemaphore* handle;

    __asm__ volatile("mv %0, a1" : "=r"(handle));

    delete handle;


    Riscv::w_a0(0);
}

void KSemaphore::semWaitSCHandler(){

    KSemaphore* handle;

    __asm__ volatile("mv %0, a1" : "=r"(handle));

    handle->wait();


    Riscv::w_a0(0);
    return;

}

void KSemaphore::semSignalSCHandler(){

    KSemaphore* handle;

    __asm__ volatile("mv %0, a1" : "=r"(handle));

    handle->signal();

    Riscv::w_a0(0);
    return;

}
#include "../h/TCB.hpp"
#include "../h/Scheduler.hpp"
#include "../h/MemAllocator.hpp"
#include "../h/syscall_c.hpp"

#include "../h/print.hpp"


Queue<TCB*> Scheduler::queueTCB;
Queue<TCB*> Scheduler::sleeping;

TCB* TCB::running = nullptr;

uint64  TCB::timeSliceCounter = 0;

int TCB::counter_id = 0;


TCB::TCB(Body body, void* args, uint64* stack, uint64 timeSlice): body(body), args(args),
                    stack(stack),
                    context({(uint64)&ThreadWrapper,
                            stack != nullptr ? (uint64) ((uint8*)stack+ DEFAULT_STACK_SIZE): 0}),
                            timeSlice(timeSlice),
                            finished(false)
{
    this->thread_id = counter_id++;

}

void TCB::start(){
    Scheduler::put(this);
}

void TCB::stop() {
    TCB* old = running;
    running = Scheduler::get();
    TCB::contextSwitch(&old->context, &running->context);
}

void TCB::dispatch()
{
    TCB* old = running;
    if (!old->isFinished()) { Scheduler::put(old); }
    running = Scheduler::get();
    TCB::contextSwitch(&old->context, &running->context);
}

void TCB::ThreadWrapper() {

    Riscv::popSppSpie();
    //USER SPACE
    running->body(running->args);
    running->setFinished(true);
    //ovde nam isto treba kontekst switch
    thread_exit();
}

//SC

void TCB::threadCreateSCHandler() {
    uint64 start_routine;
    uint64 args;
    TCB **threadHandle;
    __asm__ volatile("mv %0, a3" : "=r"(args));
    __asm__ volatile("mv %0, a2" : "=r"(start_routine));
    __asm__ volatile("mv %0, a1" : "=r"(threadHandle));



    uint64* thread_stack =(uint64*) MemoryAllocator::kmalloc(DEFAULT_STACK_SIZE);

    if(thread_stack == nullptr){
        Riscv::w_a0((uint64) -1);
        return;
    }

    TCB* newTCB = new TCB((void (*)(void *)) start_routine, (void *) args,thread_stack, DEFAULT_TIME_SLICE);
    newTCB->start();

    (*threadHandle) = newTCB;

    Riscv::w_a0( 0);

}

void TCB::threadExitSCHandler() {
    TCB::timeSliceCounter = 0;
    TCB::running->finished = true ;
    TCB::dispatch();
    __asm__ volatile("li a0, 0");
    Riscv::w_a0( 0);
}

void TCB::threadDispatchSCHandler() {

    dispatch();
}

void TCB::threadDeleteSCHandler() {
    TCB* threadHandle = nullptr;
    __asm__ volatile("mv %[threadHandle],a1" : [threadHandle]"=r"(threadHandle));
    int res = MemoryAllocator::kfree(threadHandle);
    Riscv::w_a0(res);
    return ;
}

void TCB::threadSleepSCHandler() {

    uint64 time ;
    time=Riscv::r_a_stack(1);


    Scheduler::sleepThread(TCB::running,time);
    stop();

    uint64 result=0x01;
    __asm__ volatile("mv a0,%0":"=r"(result));
    return;
}

void TCB::threadBuildSCHandler() {

    uint64 start_routine;
    uint64 args;
    TCB **threadHandle;
    __asm__ volatile("mv %0, a3" : "=r"(args));
    __asm__ volatile("mv %0, a2" : "=r"(start_routine));
    __asm__ volatile("mv %0, a1" : "=r"(threadHandle));

    uint64* thread_stack = (uint64*) MemoryAllocator::kmalloc(DEFAULT_STACK_SIZE);

    if(thread_stack == nullptr){
        Riscv::w_a0((uint64) -1);
        return;
    }

    TCB* newTCB = new TCB((void (*)(void *)) start_routine, (void *) args,thread_stack, DEFAULT_TIME_SLICE);

    (*threadHandle) = newTCB;
    Riscv::w_a0( 0);
}

void* TCB::operator new(size_t size) {
    return MemoryAllocator::kmalloc(size);
}

void TCB::operator delete(void *addr) {
    MemoryAllocator::kfree(addr);
}
//
// Created by os on 4/2/25.
//
#include "../h/Kernel.hpp"
#include "../h/riscv.hpp"
#include "../h/TCB.hpp"
#include "../h/print.hpp"
#include "../h/KConsole.hpp"
#include "../lib/console.h"
#include "../h/syscall_c.hpp"
// console.lib provides `__putc`, but it routes through `console_write()` which
// dispatches via `devsw[CONSOLE_DEV].write`. Since we don't include the full
// xv6-like device layer, we must initialize `devsw` ourselves during boot.



extern void userMain();
extern void userMainTest();

void Kernel::userMainWrapper(void* args){


    userMain();
    //userMainTest();

}

void Kernel::Init() {

    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);


    MemoryAllocator::Init();
    KConsole::initialize();

    TCB* kernelThread = nullptr;
    kernelThread = new TCB(nullptr,nullptr,TCB::SystemThread,0);
    TCB::running = kernelThread;  

    TCB* userThread = nullptr;
    userThread = new TCB(userMainWrapper, nullptr,TCB::UserThread);
    userThread->start();

    TCB* consoleThread = nullptr;
    consoleThread = new TCB(KConsole::outputConsoleThread,nullptr,TCB::SystemThread);
    consoleThread->start();

    Riscv::enableInterrupts();

    KConsole::kprintln("Kernel started...");

    while(!userThread->finished){
        TCB::dispatch();
        //KConsole::kprintln("Kernel dispatching...");
    }
    KConsole::kprintln("User thread finished");

    time_sleep(5);
}


int main(){


    Kernel::Init();
    Kernel::stopEmulator();
    return 0;
}
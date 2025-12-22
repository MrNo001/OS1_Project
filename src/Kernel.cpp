//
// Created by os on 4/2/25.
//
#include "../h/Kernel.hpp"
#include "../h/riscv.hpp"
#include "../h/TCB.hpp"
#include "../h/print.hpp"
#include "../h/KConsole.hpp"

// console.lib provides `__putc`, but it routes through `console_write()` which
// dispatches via `devsw[CONSOLE_DEV].write`. Since we don't include the full
// xv6-like device layer, we must initialize `devsw` ourselves during boot.



extern void userMain();
void Kernel::userMainWrapper(void* args){


    userMain();

}

void Kernel::Init() {

    debug_println("Starting Kernel...");
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);

    MemoryAllocator::Init();
    KConsole::initialize();
    
    TCB* kernelThread = nullptr;
    kernelThread = new TCB(nullptr,nullptr,TCB::SystemThread,0);
    kernelThread->start();
    debug_println("Kernel thread started...");
    TCB* userThread = nullptr;
    userThread = new TCB(userMainWrapper, nullptr,TCB::UserThread);
    userThread->start();
    debug_println("User thread started...");

    // TCB* consoleThread = nullptr;
    // consoleThread = new TCB(KConsole::outputConsoleThread,nullptr,TCB::SystemThread);
    // consoleThread->start();

    TCB::running = kernelThread;

    Riscv::enableInterrupts();
    debug_println("Kernel started...");

    while(!userThread->finished){
        TCB::dispatch();
    }
}


int main(){


    initConsoleLib();

    Kernel::Init();
    //Kernel::stopEmulator();


    return 0;
}
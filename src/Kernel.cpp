//
// Created by os on 4/2/25.
//
#include "../h/Kernel.hpp"
#include "../h/riscv.hpp"
#include "../h/TCB.hpp"
#include "../h/print.hpp"
#include "../h/KConsole.hpp"


extern void userMain();
void Kernel::userMainWrapper(void* args){

    userMain();
}

void Kernel::Init() {

    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);

    MemoryAllocator::Init();

    TCB* kernelThread = nullptr;
    kernelThread = new TCB(nullptr,nullptr,TCB::SystemThread,0);

    TCB* userThread = nullptr;
    userThread = new TCB(userMainWrapper, nullptr,TCB::UserThread);

    TCB* kernelThread = nullptr;
    kernelThread = new TCB(KConsole::outputConsoleThread,nullptr,TCB::SystemThread);


    TCB::running = kernelThread;

    Riscv::enableInterrupts();

    while(!userThread->finished){
        TCB::dispatch();
    }
}



int main(){

    Kernel::Init();
    Kernel::stopEmulator();

    return 0;
}
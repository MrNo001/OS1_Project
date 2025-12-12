
#include "../h/riscv.hpp"
#include "../lib/console.h"
#include "../h/print.hpp"
#include "../h/TCB.hpp"
#include "../h/KSemaphore.hpp"
#include "../h/KConsole.hpp"
#include "../h/Scheduler.hpp"




char *int_to_hex(int num) {
    static char buffer[sizeof(int) * 2 + 1]; // Buffer to hold hex string
    const char hex_chars[] = "0123456789ABCDEF"; // Hex characters
    unsigned int n = (unsigned int)num; // Cast to unsigned to handle negatives

    if (n == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return buffer;
    }

    // Count the number of hex digits
    int digits = 0;
    unsigned int temp = n;
    while (temp != 0) {
        digits++;
        temp >>= 4;
    }

    // Null-terminate the string
    buffer[digits] = '\0';

    // Fill the buffer from most significant digit to least
    for (int i = 0; i < digits; i++) {
        int shift = (digits - 1 - i) * 4;
        unsigned int nibble = (n >> shift) & 0xF;
        buffer[i] = hex_chars[nibble];
    }

    return buffer;
}



uint64 Riscv::system_time = 0;

void Riscv::w_a0 (uint64 value) {
    uint64 addr = TCB::running->saved_sp + 80;
    __asm__ volatile ("sd %[value],0(%[addr])" : : [addr]"r"(addr),[value]"r"(value));
}

uint64 Riscv::r_a_stack(int index){
    uint64 addr = TCB::running->saved_sp + 80 + 8*index;
    uint64 result;
    __asm__ volatile("ld %0, 0(%1)" : "=r"(result) : "r"(addr));
    return result;
};

void Riscv::popSppSpie() {
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::enterSystemMode(){
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}


void Riscv::handleSupervisorTrap() {

    TCB::running->saved_sp = r_sscratch();
    volatile uint64 scause = r_scause();

    if(scause == CONSOLE_HANDLER_CAUSE) {
        volatile uint64  sepc = r_sepc();
        volatile uint64 sstatus = r_sstatus();
        console_handler();
        w_sstatus(sstatus);
        w_sepc(sepc);
        return;
    }
    if(scause == TIMER_CAUSE){
        Riscv::system_time++;
        TCB::timeSliceCounter++;
        Scheduler::wakeSleeping();
        if(TCB::timeSliceCounter >= TCB::running->getTimeSlice())
        {
            volatile uint64  sepc = r_sepc();
            volatile uint64 sstatus = r_sstatus();
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        mc_sip(SIP_SSIP);
        return;
    }
    if ((scause == ECALL_SYSTEM_CAUSE) || (scause == ECALL_USER_CAUSE)) {

        volatile uint64 op_code;
        __asm__ volatile("mv %0,a0":"=r"(op_code));

        volatile uint64 sepc;
        sepc=r_sepc();
        volatile uint64 sstatus = r_sstatus();

        switch (op_code) {
            case 0x01: MemoryAllocator::memAllocSCHandler(); break;
            case 0x02: MemoryAllocator::memFreeSCHandler(); break;
            case 0x03: MemoryAllocator::memGetFreeSpaceSCHAndler(); break;
            case 0x04: MemoryAllocator::memGetLargestFreeBlockSCHandler(); break;

            case 0x11: TCB::threadCreateSCHandler(); break;
            case 0x12: TCB::threadExitSCHandler(); break;
            case 0x13: TCB::threadDispatchSCHandler(); break;
            case 0x14: TCB::threadDeleteSCHandler(); break;
            case 0x15: TCB::threadBuildSCHandler(); break;

            case 0x21:KSemaphore::semOpenSCHandler(); break;
            case 0x22:KSemaphore::semCloseSCHandler(); break;
            case 0x23:KSemaphore::semWaitSCHandler(); break;
            case 0x24:KSemaphore::semSignalSCHandler(); break;

            case 0x31:TCB::threadSleepSCHandler(); break;

            case 0x41:KConsole::getcSCHandler(); break;
            case 0x42:KConsole::putcSCHandler(); break;
            case 0x43:KConsole::getOutputBufferSCHandler();

        }
        sepc = sepc + 4;
        w_sepc(sepc);
        w_sstatus(sstatus);


        return;
    }

    //Todo 
    //Change so it stops the emulator
    printString("Unexpected: ");
    printIntegerNewLine(r_scause());
    printString(int_to_hex(r_sepc()));
    printString("\n");
    printIntegerNewLine(r_sstatus());

    while(1){

    }

    return;

}




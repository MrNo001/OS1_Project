
#ifndef OS_PROJECT_RISCV_HPP
#define OS_PROJECT_RISCV_HPP

#include "../lib/hw.h"

#include "MemAllocator.hpp"

class Riscv
{
public:

    static uint64 system_time;

    static void enterSystemMode();
    static void enableInterrupts();
    static void disableInterrupts();

    // pop sstatus.spp and sstatus.spie bits (has to be a non inline function)
    static void popSppSpie();


    static uint64 r_scause();
    static void w_scause(uint64 scause);


    static uint64 r_sepc();
    static void w_sepc(uint64 sepc);

    static uint64 r_stvec();
    static void w_stvec(uint64 stvec);


    static uint64 r_stval();
    static void w_stval(uint64 stval);

    enum BitMaskSip
    {
        SIP_SSIP = (1 << 1),
        SIP_STIP = (1 << 5),
        SIP_SEIP = (1 << 9),
    };

    static void ms_sip(uint64 mask);
    static void mc_sip(uint64 mask);

    static uint64 r_sip();
    static void w_sip(uint64 sip);


    enum BitMaskSstatus
    {
        SSTATUS_SIE = (1 << 1),
        SSTATUS_SPIE = (1 << 5),
        SSTATUS_SPP = (1 << 8),
    };

    enum scauseOptions {
        SCAUSE_BNT = 1UL<<63, // interrupt type, 1=external, 0=internal
        USER_INT = 8UL,
        SYSTEM_INT = 9UL,
    };


    static void ms_sstatus(uint64 mask);
    static void mc_sstatus(uint64 mask);


    static uint64 r_sstatus();
    static void w_sstatus(uint64 sstatus);


    static uint64 r_sscratch();

    static void w_a0(uint64 value);

    static uint64 r_a_stack(int i);

    static void supervisorTrap();

private:

    static void handleSupervisorTrap();

    static const uint64 TIMER_CAUSE = 0x8000000000000001;
    static const uint64 CONSOLE_HANDLER_CAUSE = 0x8000000000000009;

};

inline uint64 Riscv::r_scause()
{
    uint64 volatile scause;
    __asm__ volatile ("csrr %[scause], scause" : [scause] "=r"(scause));
    return scause;
}

inline void Riscv::w_scause(uint64 scause)
{
    __asm__ volatile ("csrw scause, %[scause]" : : [scause] "r"(scause));
}

inline uint64 Riscv::r_sepc()
{
    uint64 volatile sepc;
    __asm__ volatile ("csrr %[sepc], sepc" : [sepc] "=r"(sepc));
    return sepc;
}

inline void Riscv::w_sepc(uint64 sepc)
{
    __asm__ volatile ("csrw sepc, %[sepc]" : : [sepc] "r"(sepc));
}

inline uint64 Riscv::r_stvec()
{
    uint64 volatile stvec;
    __asm__ volatile ("csrr %[stvec], stvec" : [stvec] "=r"(stvec));
    return stvec;
}

inline void Riscv::w_stvec(uint64 stvec)
{
    __asm__ volatile ("csrw stvec, %[stvec]" : : [stvec] "r"(stvec));
}

inline uint64 Riscv::r_stval()
{
    uint64 volatile stval;
    __asm__ volatile ("csrr %[stval], stval" : [stval] "=r"(stval));
    return stval;
}

inline void Riscv::w_stval(uint64 stval)
{
    __asm__ volatile ("csrw stval, %[stval]" : : [stval] "r"(stval));
}

inline void Riscv::ms_sip(uint64 mask)
{
    __asm__ volatile ("csrs sip, %[mask]" : : [mask] "r"(mask));
}

inline void Riscv::mc_sip(uint64 mask)
{
    __asm__ volatile ("csrc sip, %[mask]" : : [mask] "r"(mask));
}

inline uint64 Riscv::r_sip()
{
    uint64 volatile sip;
    __asm__ volatile ("csrr %[sip], sip" : [sip] "=r"(sip));
    return sip;
}

inline void Riscv::w_sip(uint64 sip)
{
    __asm__ volatile ("csrw sip, %[sip]" : : [sip] "r"(sip));
}

inline void Riscv::ms_sstatus(uint64 mask)
{
    __asm__ volatile ("csrs sstatus, %[mask]" : : [mask] "r"(mask));
}

inline void Riscv::mc_sstatus(uint64 mask)
{
    __asm__ volatile ("csrc sstatus, %[mask]" : : [mask] "r"(mask));
}

inline uint64 Riscv::r_sstatus()
{
    uint64 volatile sstatus;
    __asm__ volatile ("csrr %[sstatus], sstatus" : [sstatus] "=r"(sstatus));
    return sstatus;
}

inline void Riscv::w_sstatus(uint64 sstatus)
{
    __asm__ volatile ("csrw sstatus, %[sstatus]" : : [sstatus] "r"(sstatus));
}

inline uint64 Riscv::r_sscratch(){
    uint64 volatile sscratch;
    __asm__ volatile ("csrr %[sscratch],sscratch" : [sscratch] "=r" (sscratch));
    return sscratch;
}

inline void Riscv::enableInterrupts() {
    ms_sstatus(Riscv::SSTATUS_SIE);
}

inline void Riscv::disableInterrupts(){
    mc_sstatus(Riscv::SSTATUS_SIE);
}


#endif //OS_PROJECT_RISCV_HPP

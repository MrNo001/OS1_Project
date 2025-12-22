//
// Created by os on 4/2/25.
//

#ifndef OS_PROJECT_KERNEL_HPP
#define OS_PROJECT_KERNEL_HPP


#include "../lib/hw.h"

class Kernel{

public:
    static void Init();
    static void userMainWrapper(void* args);
    static void stopEmulator();

    static uint64 syscall( uint64 op, uint64 a1 = 0, uint64 a2 = 0, uint64 a3 = 0);
};

inline void Kernel::stopEmulator(){

    __asm__ volatile (
    "li t0, 0x5555\n\t"
    "li t1, 0x100000\n\t"
    "sw t0, 0(t1)"  
    );

}

inline uint64 Kernel::syscall(uint64 op,uint64 arg1,uint64 arg2, uint64 arg3) {
    uint64 ret;
    __asm__ volatile(
        "mv a0, %[op]\n"
        "mv a1, %[arg1]\n"
        "mv a2, %[arg2]\n"
        "mv a3, %[arg3]\n"
        "ecall\n"
        "mv %[ret], a0\n"
        : [ret] "=r"(ret)
        : [op] "r"(op),
          [arg1] "r"(arg1),
          [arg2] "r"(arg2),
          [arg3] "r"(arg3)
        : "a0", "a1", "a2", "a3", "memory"
    );
    return ret;
}


#endif //OS_PROJECT_KERNEL_HPP

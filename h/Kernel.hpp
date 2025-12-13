//
// Created by os on 4/2/25.
//

#ifndef OS_PROJECT_KERNEL_HPP
#define OS_PROJECT_KERNEL_HPP


class Kernel{

private:
    static void Init();
    static void userMainWrapper(void* args);

public:
    static void stopEmulator();
};

inline void Kernel::stopEmulator(){

    __asm__ volatile (
    "li t0, 0x5555\n\t"
    "li t1, 0x100000\n\t"
    "sw t0, 0(t1)"
);

}

#endif //OS_PROJECT_KERNEL_HPP

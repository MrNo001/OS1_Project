//
// Created by os on 7/3/25.
//

#ifndef OS_PROJECT_KCONSOLE_HPP
#define OS_PROJECT_KCONSOLE_HPP

#include "Queue.hpp"
#include "KSemaphore.hpp"
#include "CharBuffer.hpp"

class KConsole{

private:
    static Queue<char> outputBuffer;

    static void outputConsoleThread(void* unused);

    static void getCharactersFromConsole();

    static const uint64 STATUS_READ_MASK = 1UL;
    static const uint64 STATUS_WRITE_MASK = 1UL << 5UL;
    static const uint64 CONSOLE_GETC = 0x41;
    static const uint64 CONSOLE_PUTC = 0x42;
    static const uint64 CONSOLE_GET_CHAR = 0x43;


    static uint64 pendingGetc;
    static uint64 pendingPutc;

    static KSemaphore* hasCharactersOutput;
    static KSemaphore* hasCharactersInput;

    static CharBuffer* input;
    static CharBuffer* output;

    static void initialize();

public:
    static void getcSCHandler();
    static void putcSCHandler();

    static void getOutputBufferSCHandler();

    static void kprintln(int x,int base = 10);
    static void kprintln(const char* string);
    static void kprint(int x,int base = 10);
    static void kprint(const char* string);

    static uint64 console_status();
    static uint64 console_receive();
    static uint64 console_send(uint64 c);
};


inline uint64 Kernel::console_status() {
    uint64 x = CONSOLE_STATUS;
    __asm__ volatile("mv a0, %0"::"r"(x));
    __asm__ volatile("lb a1, 0(a0)");
    uint64 operation;
    __asm__ volatile("mv %0, a1" :  "=r"(operation));
    return operation;
}
inline uint64 kernel::console_receive() {
    uint64 x = CONSOLE_TX_DATA;
    __asm__ volatile("mv a0, %0"::"r"(x));
    __asm__ volatile("lb a1,0(a0)");
    char c;
    __asm__ volatile("mv %0, a1" :  "=r"(c));
    return c;
}
inline void Kernel::console_send(uint64 c) {
    uint64 x = CONSOLE_RX_DATA;
    __asm__ volatile("mv a0, %0"::"r"(x));
    __asm__ volatile("mv a1, %0" :  :"r"((uint64)c));
    __asm__ volatile("sb a1,0(a0)");
}



#endif //OS_PROJECT_KCONSOLE_HPP

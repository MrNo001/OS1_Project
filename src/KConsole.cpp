//
// Created by os on 7/3/25.
//

#include "../h/CharBuffer.hpp"
#include "../h/KConsole.hpp"
#include "../lib/console.h"
#include "../h/riscv.hpp"
#include "../h/TCB.hpp"


KSemaphore* KConsole::hasCharactersOutput = 0;
KSemaphore* KConsole::hasCharactersInput = 0;


Queue<char> KConsole::outputBuffer{};

uint64 KConsole::pendingPutc;
uint64 KConsole::pendingGetc;

CharBuffer* KConsole::input = nullptr;
CharBuffer* KConsole::output = nullptr;

void KConsole::initialize()
{
    input = new CharBuffer();
    output = new CharBuffer();
}

// --------------KERNEL_PRINT-------------------

void KConsole::kprint(const char* string) {
    while (*string != '\0') {
        output->put(*string);
        string++;
    }
}

void KConsole::kprint(int x, int base) {
    bool neg = false;
    if (x < 0) neg = true, x = -x;
    char digits[] = "0123456789ABCDEF";
    char buf[16];
    int n = 0;
    do buf[n++] = digits[x % base];
    while ((x /= base) != 0);
    if (neg) buf[n++] = '-';
    while (n--) output->put(buf[n]);
}


void KConsole::kprintln(const char* string) {
    kprint(string);
    output->put('\n');
}

void KConsole::kprintln(int x, int base) {
    kprint(x,base);
    output->put('\n');
}

//INT

void KConsole::getCharactersFromConsole() {
    bool readable = KConsole::console_status() & CONSOLE_RX_STATUS_BIT;
    if (readable) {
        char c = KConsole::console_receive();
        // if (pendingGetc > 0)
        input->put(c);
    }
}

//--------------SC_HANDLER-------------------------------
void KConsole::getcSCHandler() {

    char c = input->get();
    if (c != 27) // ESC = 27
        output->put(c);
    if (c == 13) // CR = 13, LF = 10
        output->put(10);
    Riscv::w_a0((uint64)c);
    return;
}


void KConsole::getOutputBufferSCHandler() {
    char c = output->get();
    Riscv::w_a0((uint64)c);
}

void KConsole::putcSCHandler() {

    

    char c = (char) Riscv::r_a_stack(1);
    __putc(c);
    return;
    output->put(c);
}

void KConsole::debugGetcSCHandler() {
    uint64 oldStack=TCB::running->saved_sp;
    uint64 result = (uint64) __getc();
    TCB::running->saved_sp = oldStack;
    Riscv::w_a0(result);
    return;
}

void KConsole::debugPutcSCHandler() {
    uint64 c = Riscv::r_a_stack(1);
    uint64 oldStack=TCB::running->saved_sp;
    __putc((char)c);
    TCB::running->saved_sp = oldStack;
    return;
}

//------------THREAD--------------------

#include "../h/syscall_c.hpp"
void KConsole::outputConsoleThread(void* unused) {
     while(true) {

        bool writable = KConsole::console_status() & CONSOLE_TX_STATUS_BIT;
        if (writable && !output->empty()) {
            char volatile c = get_buffer_output();
            KConsole::console_send(c);
        }

        else {
            thread_dispatch();
        }
    }
}


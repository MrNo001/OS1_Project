//
// Created by marko on 20.4.22..
//

#include "../h/print.hpp"
#include "../h/syscall_c.hpp"
#include "../h/riscv.hpp"


//Todo,change this to reflect the implementation of putc

void println(char const *string)
{
    uint64 sstatus = Riscv::r_sstatus();
    Riscv::mc_sstatus(Riscv::SSTATUS_SIE);
    while (*string != '\0')
    {
        putc(*string);
        string++;
    }
    putc('\n');
    Riscv::ms_sstatus(sstatus & Riscv::SSTATUS_SIE ? Riscv::SSTATUS_SIE : 0);
}

void print(char const *string)
{
    uint64 sstatus = Riscv::r_sstatus();
    Riscv::mc_sstatus(Riscv::SSTATUS_SIE);
    while (*string != '\0')
    {
        putc(*string);
        string++;
    }
    Riscv::ms_sstatus(sstatus & Riscv::SSTATUS_SIE ? Riscv::SSTATUS_SIE : 0);
}

void print(uint64 integer)
{
    uint64 sstatus = Riscv::r_sstatus();
    Riscv::mc_sstatus(Riscv::SSTATUS_SIE);
    static char digits[] = "0123456789";
    char buf[16];
    int i, neg;
    uint64 x;

    neg = 0;
    if (integer < 0)
    {
        neg = 1;
        x = -integer;
    } else
    {
        x = integer;
    }

    i = 0;
    do
    {
        buf[i++] = digits[x % 10];
    } while ((x /= 10) != 0);
    if (neg)
        buf[i++] = '-';

    while (--i >= 0) { putc(buf[i]); }
    Riscv::ms_sstatus(sstatus & Riscv::SSTATUS_SIE ? Riscv::SSTATUS_SIE : 0);
}

void println(uint64 integer){
    print(integer);
    putc('\n');
}


void debug_print(const char* string) {
    while (*string != '\0') {
        debug_putc(*string);
        string++;
    }
}

void debug_println(const char* string) {
    debug_print(string);
    debug_putc('\n');
}

void debug_print(int x,int base) {
    char digits[] = "0123456789ABCDEF";
    char buf[16];
    int n = 0;
    do buf[n++] = digits[x % base];
    while ((x /= base) != 0);
    while (n--) debug_putc(buf[n]);
}

void debug_println(int x,int base) {
    debug_print(x,base);
    debug_putc('\n');
}

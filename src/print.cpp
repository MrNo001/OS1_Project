#include "../h/print.hpp"
#include "../h/syscall_c.hpp"
#include "../h/riscv.hpp"

void print(char const *string)
{
    while (*string != '\0')
    {
        putc(*string);
        string++;
    }
    time_sleep(time_t(2));
}

void print(uint64 integer)
{
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

    time_sleep(time_t(2));
   
}




void debug_print(const char* string) {
    while (*string != '\0') {
        debug_putc(*string);
        string++;
    }
}

void debug_print(int x,int base) {
    char digits[] = "0123456789ABCDEF";
    char buf[16];
    int n = 0;
    do buf[n++] = digits[x % base];
    while ((x /= base) != 0);
    while (n--) debug_putc(buf[n]);
}

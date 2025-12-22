
#ifndef PRINT_HPP
#define PRINT_HPP

#include "../lib/hw.h"

void println(char const* string);

void println(uint64 integer);

void print(char const* string);

void print(char const);

void debug_print(const char* string);
void debug_println(const char* string);
void debug_print(int x,int base = 10);
void debug_println(int x,int base = 10);

extern "C" {
    struct devsw_t {
        int (*read)(int user_dst, uint64 dst, int n);
        int (*write)(int user_src, uint64 src, int n);
    };

    // Provided by console.lib (BSS symbol, zero-initialized by default)
    extern devsw_t devsw[];

    // Provided by console.lib
    void consoleinit(void);
    int consoleread(int user_dst, uint64 dst, int n);
    int consolewrite(int user_src, uint64 src, int n);
}

static constexpr int CONSOLE_DEV = 1;
static inline void initConsoleLib() {
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    consoleinit();
    devsw[CONSOLE_DEV].read = consoleread;
    devsw[CONSOLE_DEV].write = consolewrite;
}

#endif 

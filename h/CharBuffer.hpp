#pragma once

#include "../h/MemAllocator.hpp"
#include "../h/KSemaphore.hpp"

class CharBuffer {

public:
    CharBuffer():
        head(0), tail(0), semaphore(KSemaphore(0))
    {}

    void initialize() {
        //semaphore = KSemaphore(0);
    }

    inline bool full() const {
        return (tail + 1) % bufferSize == head;
    }

    inline bool empty() const {
        return head == tail;
    }

    // getc - before, putc - after empy and full
    char get() {
        pendingGetc++;
        semaphore.wait();
        if (empty()) return -1;
        char c = buffer[head];
        head = (head + 1) % bufferSize;
        pendingPutc--;
        return c;
    }

    int put(char c) {
        pendingGetc--;
        if (full()) return -1;
        pendingPutc++;
        buffer[tail] = c;
        tail = (tail + 1) % bufferSize;
        semaphore.signal();
        return 0;
    }

    // memory allocation
    void* operator new(size_t size) {
        return MemoryAllocator::kmalloc(size);
    }
    void operator delete(void* ptr) {
        MemoryAllocator::kfree(ptr);
    }

private:
    static const uint64 bufferSize = 8192;
    uint64 head, tail;
    KSemaphore semaphore;
    char buffer[bufferSize];

public:
    uint64 pendingPutc = 0, pendingGetc = 0;

};


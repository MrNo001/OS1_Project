#pragma once

#include "../h/MemAllocator.hpp"
#include "../h/KSemaphore.hpp"

class CharBuffer {

public:
    CharBuffer():
        queue(), semaphore(KSemaphore(0))
    {}

    inline bool empty() const {
        return queue.isEmpty();
    }

    char get() {
        semaphore.wait();
        if (queue.isEmpty()) return -1;
        char c = queue.pop();
        return c;
    }

    int put(char c) {
        queue.put(c);
        semaphore.signal();
        return 0;
    }

    void* operator new(size_t size) {
        return MemoryAllocator::kmalloc(size);
    }
    void operator delete(void* ptr) {
        MemoryAllocator::kfree(ptr);
    }

private:
    Queue<char> queue;
    KSemaphore semaphore;
 

};


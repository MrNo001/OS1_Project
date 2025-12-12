//
// Created by os on 1/3/25.
//

#ifndef OS_PROJECT_MEMALLOCATOR_HPP
#define OS_PROJECT_MEMALLOCATOR_HPP

#include "../lib/hw.h"


class MemoryAllocator{

private:

    struct MemHeader{

        size_t mem_size;
        MemHeader* next;
        MemHeader* previous;

    };

    static MemHeader* free_list_head;
    static MemHeader* alloc_list_head;

    static void Init();

    static void* mem_alloc(size_t size);
    static int mem_free(void*);
    static size_t mem_get_free_space();
    static size_t mem_get_largest_free_block();

    static bool isValidMemSegment(MemHeader* mem);
    static void mergeFreeSegments(MemHeader* previous,MemHeader* current,MemHeader* next);

    static void* kmalloc(size_t size);
    static int kfree(void*);


    static void memAllocSCHandler();
    static void memFreeSCHandler();
    static void memGetLargestFreeBlockSCHandler();
    static void memGetFreeSpaceSCHAndler();


    friend class Riscv;


};


#endif //OS_PROJECT_MEMALLOCATOR_HPP

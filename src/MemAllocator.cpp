//
// Created by os on 1/3/25.
//
#include "../h/MemAllocator.hpp"
#include "../h/print.hpp"
#include "../h/riscv.hpp"

#define SIZE_OF_MEM_HEADER sizeof(MemHeader)

uint64 MemoryAllocator::sizeAvailable = (uint64)HEAP_END_ADDR - (uint64)HEAP_START_ADDR;


MemoryAllocator::MemHeader* MemoryAllocator::free_list_head;

MemoryAllocator::MemHeader* MemoryAllocator::alloc_list_head;

MemoryAllocator::MemoryAllocator(){
   MemoryAllocator::Init();
}

void MemoryAllocator::Init(){
    free_list_head=(MemHeader*)HEAP_START_ADDR;
    free_list_head->mem_size=(uint64)HEAP_END_ADDR-(uint64)HEAP_START_ADDR-sizeof(MemHeader);
    free_list_head->next=nullptr;
    free_list_head->previous=nullptr;
    alloc_list_head = nullptr;
}

void* MemoryAllocator::mem_alloc(size_t size){

    size_t size_required=((size+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE)*MEM_BLOCK_SIZE;

    MemHeader* previous = nullptr;
    MemHeader* current = free_list_head;
    while(current && current->mem_size <= size_required){
        previous = current;
        current = current->next;
    }
    if(current == nullptr){
        printString("No more space for ");
        printIntegerNewLine(size_required);
        return nullptr;
    }

    if(previous) previous->next = current->next;

    uint64 remaining_size = current->mem_size - size_required;

    if(remaining_size < sizeof(MemHeader)){

        if(previous) previous->next = current->next;
        else free_list_head = current->next;
        if(current->next) current->next->previous = previous;

    }
    else {
        MemHeader *new_free = (MemHeader *) ((uint8 *) current + sizeof(MemHeader) + size_required);
        new_free->mem_size = current->mem_size - size_required - sizeof(MemHeader);
        new_free->next = current->next;
        new_free->previous = previous;

        if (previous) previous->next = new_free;
        else free_list_head = new_free;
    }

    current->next = alloc_list_head;
    current->previous = nullptr;
    if(alloc_list_head != nullptr) alloc_list_head->previous = current;
    alloc_list_head = current;
    current->mem_size = size_required;

    void* addr = (void*)((uint8*)current + sizeof(MemHeader));
    if(addr<=HEAP_START_ADDR || addr>=HEAP_END_ADDR){
        printString("Illegal address");
        printIntegerNewLine((uint64)addr);
    }
    numOfAllocations++;
    return addr;

}


bool MemoryAllocator::isValidMemSegment(MemHeader* mem_segment){

    for(MemHeader* current = alloc_list_head; current != nullptr; current = current->next){
        if(current == mem_segment){
            return true;
        }
    }
    return false;
}


void MemoryAllocator::mergeFreeSegments(MemHeader* prev,MemHeader* curr,MemHeader* next){

    // Merge with next block if adjacent
    if (next && (uint8*)curr + sizeof(MemHeader) + curr->mem_size == (uint8*)next) {
        curr->mem_size += sizeof(MemHeader) + next->mem_size;
        curr->next = next->next;
        if (next->next) next->next->previous = curr;
    }

    // Merge with previous block if adjacent
    if (prev && (uint8*)prev + sizeof(MemHeader) + prev->mem_size == (uint8*)curr) {
        prev->mem_size += sizeof(MemHeader) + curr->mem_size;
        prev->next = curr->next;
        if (curr->next) curr->next->previous = prev;
    }

}


int MemoryAllocator::mem_free(void* mem_to_free){

    MemHeader* mem_block = (MemHeader*)((uint8*)mem_to_free-sizeof(MemHeader));
    MemHeader* alloc_next = mem_block->next;
    MemHeader* alloc_previous = mem_block->previous;

    if(!isValidMemSegment(mem_block)){
        return -1;
    }

    if (alloc_next != nullptr) {
        alloc_next -> previous = alloc_previous;
    }
    if(alloc_previous != nullptr){
        alloc_previous ->next = alloc_next;
    }
    else{
      alloc_list_head = alloc_next;
    }

    MemHeader* current = free_list_head;
    MemHeader* previous = nullptr;

    while (current && (uint8*)current < (uint8*)mem_block) {
        previous = current;
        current = current->next;
    }

    mem_block->next = current;
    mem_block->previous = previous;

    if (previous)
        previous->next = mem_block;
    else
        free_list_head = mem_block; // New head

    if (current)
        current->previous = mem_block;

// Merge with neighbors
    mergeFreeSegments(previous, mem_block, current);
    return 1;
}


size_t MemoryAllocator::mem_get_free_space(){
    size_t free_space = 0;
    for(MemHeader* current = free_list_head; current != nullptr; current = current->next ){
        free_space += current->mem_size;
    }
    return free_space;
}

size_t MemoryAllocator::mem_get_largest_free_block(){
    size_t largest_free_block_size = 0;
    for(MemHeader* current = free_list_head; current != nullptr; current = current->next ){
        if(current->mem_size > largest_free_block_size) largest_free_block_size = current->mem_size;
    }
    return largest_free_block_size;
}


void MemoryAllocator::memAllocSCHandler(){
    size_t size;
    __asm__ volatile (" mv %[size],a1" : [size]"=r"(size));
    void* addr = mem_alloc(size);
    Riscv::w_a0((uint64)addr );
}

void MemoryAllocator::memFreeSCHandler() {
    uint64 addr;
    __asm__ volatile (" mv %[addr],a1" : [addr]"=r"(addr));
    int result = mem_free((void*) addr);
    Riscv::w_a0((uint64) result );
}

void MemoryAllocator::memGetFreeSpaceSCHAndler(){
    size_t result = MemoryAllocator::mem_get_free_space();
    Riscv::w_a0((uint64) result);
}

void MemoryAllocator::memGetLargestFreeBlockSCHandler(){
    size_t result = MemoryAllocator::mem_get_largest_free_block();
    Riscv::w_a0((uint64) result);
}

void* MemoryAllocator::kmalloc(size_t size){
    return mem_alloc(size);
}

int MemoryAllocator::kfree(void* mem_to_free){
    return mem_free(mem_to_free);
}
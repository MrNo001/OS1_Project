//
// Created by os on 1/7/25.
//



#include "../h/syscall_c.hpp"
#include "../h/Kernel.hpp"

//---------------MEMORY-------------------------

void* mem_alloc(size_t size){
    return (void*) Kernel::syscall(0x01, (uint64)size);
}

int mem_free(void* memory_to_free){
    return Kernel::syscall(0x02, (uint64)memory_to_free);
}


size_t mem_get_free_space(){
    return Kernel::syscall(0x03);
}

size_t mem_get_largest_free_block(){
    return Kernel::syscall(0x04);
}

//---------------THREAD-------------------------

int thread_create(thread_t* handle, void (*start_routine)(void*), void* args){
    return Kernel::syscall(0x11, (uint64)handle, (uint64)start_routine, (uint64)args);
}

int thread_exit(){
    return Kernel::syscall(0x12);
}

void thread_dispatch(){
    Kernel::syscall(0x13);
}

int thread_delete(thread_t handle){
    return Kernel::syscall(0x14, (uint64)handle);
}

int thread_build(thread_t* handle, void (*start_routine)(void*), void* args){
    return Kernel::syscall(0x15, (uint64)handle, (uint64)start_routine, (uint64)args);
}

int thread_start(thread_t handle){
    return Kernel::syscall(0x16, (uint64)handle);
}

//---------------SEMAPHORE-------------------------

int sem_open(sem_t* handle, unsigned init){
    return Kernel::syscall(0x21, (uint64)handle, (uint64)init);
}

int sem_close(sem_t handle){
    return Kernel::syscall(0x22, (uint64)handle);
}

int sem_wait(sem_t id){
    return Kernel::syscall(0x23, (uint64)id);
}

int sem_signal(sem_t id){
    return Kernel::syscall(0x24, (uint64)id);
}

//-----------------SLEEP---------------------------------

int time_sleep (time_t time){
    return Kernel::syscall(0x31, (uint64)time);
}

//--------------CONSOLE------------------------------

char getc(){
    return Kernel::syscall(0x41);
}

void putc(char c){
    Kernel::syscall(0x42, (uint64)c);
}

char get_buffer_output(){
    return (char) Kernel::syscall(0x43);
}
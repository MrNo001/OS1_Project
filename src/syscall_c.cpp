//
// Created by os on 1/7/25.
//



#include "../h/syscall_c.hpp"


//---------------MEMORY-------------------------

void* mem_alloc(size_t size){

    uint64 op_code = 0x01;
    __asm__ volatile("mv a0, %[op_code]" : : [op_code] "r" (op_code));
    __asm__ volatile("mv a1, %[size]" :  : [size]"r"((uint64)size));


    __asm__ volatile("ecall");


    uint64 addr=0x01;
    __asm__ volatile("mv %0,a0":"=r"(addr));

    return (void*) addr;

}

int mem_free(void* memory_to_free){
    uint64 op_code = 0x02;
    __asm__ volatile("mv a0, %[op_code]" : : [op_code] "r" (op_code));
    __asm__ volatile("mv a1, %[memory_to_free]": : [memory_to_free] "r" ((uint64)memory_to_free));
    __asm__ volatile("ecall");

    uint64 result;
    __asm__ volatile("mv %[result],a0" : [result]"=r"(result));

    return result;
}


size_t mem_get_free_space(){
    uint64 op_code = 0x03;
    __asm__ volatile ("mv a0,%[op_code]" : : [op_code] "r" (op_code));
    __asm__ volatile ("ecall");

    size_t result;
    __asm__ volatile("mv %[result],a1" : [result]"=r"(result));
    return result;
}

size_t mem_get_largest_free_block(){
    uint64 op_code = 0x04;
    __asm__ volatile ("mv a0,%[op_code]" : : [op_code] "r" (op_code));
    __asm__ volatile ("ecall");

    size_t result;
    __asm__ volatile("mv %[result],a1" : [result]"=r"(result));
    return result;
}

//---------------THREAD-------------------------

int thread_create(thread_t* handle, void (*start_routine)(void*), void* args){

    uint64 op_code = 0x11;

    __asm__ volatile("mv a0, %[op_code]" :  : [op_code] "r" ((uint64)op_code));

    __asm__ volatile("mv a1, %0" :  : "r" ((uint64)handle));
    __asm__ volatile("mv a2, %0" :  : "r" ((uint64)start_routine));
    __asm__ volatile("mv a3, %0" :  : "r" ((uint64)args));
    __asm__ volatile("ecall");

    uint64 result;
    __asm__ volatile("mv %[result], a0" : [result] "=r"(result));

    return result;

}

int thread_exit(){
    uint64 op_code = 0x12;
    __asm__ volatile("mv a0,%[op_code]" : : [op_code]"r"((uint64)op_code));
    __asm__ volatile ("ecall");

    uint64 result;
    __asm__ volatile("mv %[result], a0" : [result] "=r"(result));
    return result;
}

void thread_dispatch(){
    uint64 op_code = 0x13;
    __asm__("mv a0, %[op_code]" : : [op_code]"r"((uint64)op_code));
    __asm__("ecall");
    return;
}

int thread_delete(thread_t handle){
    uint64 op_code = 0x14;

    __asm__ volatile("mv a0, %[op_code]" : : [op_code]"r"((uint64)op_code));
    __asm__ volatile("mv a1, %[handle_param]" : : [handle_param]"r"((uint64)handle));
    return 1;
}

int thread_build(thread_t* handle, void (*start_routine)(void*), void* args){
    
    uint64 op_code = 0x15;
     __asm__ volatile("mv a0, %[op_code]" :  : [op_code] "r" ((uint64)op_code));

    __asm__ volatile("mv a3, %0" :  : "r" ((uint64)args));
    __asm__ volatile("mv a2, %0" :  : "r" ((uint64)start_routine));
    __asm__ volatile("mv a1, %0" :  : "r" ((uint64)handle));
    __asm__ volatile("ecall");

    uint64 result;
    __asm__ volatile("mv %[result], a0" : [result] "=r"(result));

    return result;

}

int thread_start(thread_t handle){
    uint64 op_code = 0x16;
    __asm__ volatile("mv a1,%[handle]" : : [handle]"r"((uint64)handle));
    __asm__ volatile("mv a0,%[op_code]" : : [op_code]"r"(op_code));
    __asm__ volatile("ecall");

    uint64 result;
    __asm__ volatile("mv %[result], a0" : [result] "=r"(result));

    return result;

}

//---------------SEMAPHORE-------------------------

int sem_open(sem_t* handle, unsigned init){
    uint64 handle_ = (uint64)handle;
    uint64 init_ = (uint64)init;
    uint64 op_code = 0x21;


    __asm__ volatile("mv a2, %0" :  : "r" ((uint64)init_));
    __asm__ volatile("mv a1, %0" :  : "r" ((uint64)handle_));
    __asm__ volatile("mv a0, %[op_code]" :  : [op_code] "r" ((uint64)op_code));


    __asm__ volatile("ecall");

    //get the result
    uint64 result;
    __asm__ volatile("mv %[result], a0" : [result] "=r"(result));

    return result;
}

int sem_close(sem_t handle){

    uint64 handle_ = (uint64)handle;
    uint64 op_code = 0x22;

    __asm__ volatile("mv a1, %0" :  : "r" ((uint64)handle_));
    __asm__ volatile("mv a0, %[op_code]" :  : [op_code] "r" ((uint64)op_code));


    __asm__ volatile("ecall");

    //get the result
    uint64 result;
    __asm__ volatile("mv %[result], a0" : [result] "=r"(result));

    return result;
}

int sem_wait(sem_t id){
    uint64 id_ = (uint64) id;
    uint64 op_code = 0x23;
    __asm__ volatile("mv a1,%[id_]" : : [id_] "r" ((uint64)id_));
    __asm__ volatile("mv a0, %[op_code]" :  : [op_code] "r" ((uint64)op_code));

    __asm__ volatile("ecall");

    uint64 result;

    __asm__ volatile("mv %[result], a0" : [result] "=r"(result));
    return result;

}

int sem_signal(sem_t id){

    uint64 id_ = (uint64) id;
    uint64 op_code = 0x24;
    __asm__ volatile("mv a1,%[id_]" : : [id_] "r" ((uint64)id_));
    __asm__ volatile("mv a0, %[op_code]" :  : [op_code] "r" ((uint64)op_code));

    __asm__ volatile("ecall");

    uint64 result;

    __asm__ volatile("mv %[result], a0" : [result] "=r"(result));
    return result;

}


//-----------------SLEEP---------------------------------

int time_sleep (time_t time){
    uint64 op_code = 0x31;
    __asm__ volatile("mv a1,%0"::"r"((uint64)time));
    __asm__ volatile("mv a0,%0"::"r"(op_code));


    __asm__ volatile("ecall");

    uint64 result;
    __asm__ volatile("mv %[result],a0":[result]"=r"(result));
    return result;
}

//--------------CONSOLE------------------------------

char getc(){
    __asm__ volatile("mv a0, %0" : : "r" (0x41));

    __asm__ volatile("ecall");

    uint64 c;
    __asm__ volatile("mv %0, a0" : "=r" (c));
    return (char)c;
}

void putc(char c){
    __asm__ volatile("mv a1, %0" : : "r" ((uint64)c));
    __asm__ volatile("mv a0, %0" : : "r" (0x42));

    __asm__ volatile("ecall");
    return;

}

char get_buffer_output(){
    __asm__ volatile("li a0, 0x43");

    __asm__ volatile("ecall");

    uint64 result;
    __asm__ volatile("mv %0, a0" : "=r"(result));
    return (char)result;
}
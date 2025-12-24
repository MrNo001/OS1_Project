#ifndef OS_PROJECT_SYSCALL_C_H
#define OS_PROJECT_SYSCALL_C_H

#include "../lib/hw.h"
#include "../lib/console.h"

//--------------------MEMORY-----------------------
void* mem_alloc(size_t size);
int mem_free(void*);
size_t mem_get_free_space();
size_t mem_get_largest_free_block();


//--------------------THREAD-----------------------
class _thread{};
typedef _thread* thread_t;

int thread_create(thread_t* handle, void (*start_routine)(void*), void* arg );
int thread_exit();
void thread_dispatch();
int thread_delete(thread_t handle);
int thread_build(thread_t* handle, void (*start_routine)(void*), void* arg);
int thread_start(thread_t handle);

//--------------------SEMAPHORE-----------------------
class _sem;
typedef _sem* sem_t;

int sem_open(sem_t* handle, unsigned int init);
int sem_close(sem_t handle);
int sem_wait(sem_t id);
int sem_signal(sem_t id);

//--------------------SLEEP-----------------------
typedef unsigned long time_t;
int time_sleep (time_t);


//--------------------CONSOLE-----------------------
const int EOF = -1;
char getc();
void putc(char);

char get_buffer_output();

char debug_getc();
void debug_putc(char);

#endif //OS_PROJECT_SYSCALL_C_H
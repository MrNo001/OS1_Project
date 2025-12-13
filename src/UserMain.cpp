//
// Created by os on 1/3/25.
//

#include "../lib/console.h"
#include "../h/MemAllocator.hpp"
#include "../h/riscv.hpp"

#include "../h/syscall_c.hpp"
#include "../h/print.hpp"
#include "../test/printing.hpp"
#include "../test/ConsumerProducer_C_API_test.hpp"
#include "../test/ThreadSleep_C_API_test.hpp"

void printMemBlock(void* location,uint64 size){

    size=((size+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE)*MEM_BLOCK_SIZE;
    printInteger((uint64)location);
    printStringLock(" Do ");
    printInteger((uint64)location+size-1);
    printStringLock("------------------");
    return;
}


void MemoryTesterDirect(){
    MemoryAllocator Alokator;

    void* Lokacija1=Alokator.mem_alloc(4096);
    void* Lokacija2=Alokator.mem_alloc(64);
    void* Lokacija3=Alokator.mem_alloc(4096);
    void* Lokacija4=Alokator.mem_alloc(64);

    printMemBlock(Lokacija1,4096);
    printMemBlock(Lokacija2,64);
    printMemBlock(Lokacija3,4096);
    printMemBlock(Lokacija4,64);

    printInteger((uint64)Alokator.mem_free(Lokacija1));
    printInteger((uint64)Alokator.mem_free(Lokacija4));
    printInteger((uint64)Alokator.mem_free(Lokacija3));
    //printInteger((uint64)Alokator.mem_free(Lokacija2));

   void* Lokacija5=Alokator.mem_alloc(250);
   void* Lokacija6=Alokator.mem_alloc(250);

    printMemBlock(Lokacija5,250);
    printMemBlock(Lokacija6,250);

}

void MemoryTester(){

    void* Lokacija1=mem_alloc(72);
    void* Lokacija2=mem_alloc(72);
    printInteger((uint64)Lokacija1);
    printInteger((uint64)Lokacija2);

}



void TestingMain(){
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);

    MemoryAllocator::Init();
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);

    MemoryTester();
}




static volatile bool finishedA = false;
static volatile bool finishedB = false;
static volatile bool finishedC = false;
static volatile bool finishedD = false;

static uint64 fibonacci(uint64 n) {
    if (n == 0 || n == 1) { return n; }
    if (n % 10 == 0) { thread_dispatch(); }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

static void workerBodyA(void* arg) {
    for (uint64 i = 0; i < 10; i++) {
        printStringLock("A: i="); printInt(i); printStringLock("\n");
        for (uint64 j = 0; j < 10000; j++) {
            for (uint64 k = 0; k < 30000; k++) { /* busy wait */ }
            thread_dispatch();
        }
    }
    printStringLock("A finished!\n");
    finishedA = true;
}

static void workerBodyB(void* arg) {
    for (uint64 i = 0; i < 16; i++) {
        printStringLock("B: i="); printInt(i); printStringLock("\n");
        for (uint64 j = 0; j < 10000; j++) {
            for (uint64 k = 0; k < 30000; k++) { /* busy wait */ }
            thread_dispatch();
        }
    }
    printStringLock("B finished!\n");
    finishedB = true;
    thread_dispatch();
}

static void workerBodyC(void* arg) {
    uint8 i = 0;
    for (; i < 3; i++) {
        printStringLock("C: i="); printInt(i); printStringLock("\n");
    }

    printStringLock("C: dispatch\n");
    __asm__ ("li t1, 7");
    thread_dispatch();

    uint64 t1 = 0;
    __asm__ ("mv %[t1], t1" : [t1] "=r"(t1));

    printStringLock("C: t1="); printInt(t1); printStringLock("\n");

    uint64 result = fibonacci(12);
    printStringLock("C: fibonaci="); printInt(result); printStringLock("\n");

    for (; i < 6; i++) {
        printStringLock("C: i="); printInt(i); printStringLock("\n");
    }

    printStringLock("A finished!\n");
    finishedC = true;
    thread_dispatch();
}

static void workerBodyD(void* arg) {
    uint8 i = 10;
    for (; i < 13; i++) {
        printStringLock("D: i="); printInt(i); printStringLock("\n");
    }

    printStringLock("D: dispatch\n");
    __asm__ ("li t1, 5");
    thread_dispatch();

    uint64 result = fibonacci(16);
    printStringLock("D: fibonaci="); printInt(result); printStringLock("\n");

    for (; i < 16; i++) {
        printStringLock("D: i="); printInt(i); printStringLock("\n");
    }

    printStringLock("D finished!\n");
    finishedD = true;
    thread_dispatch();
}

static void workerBodyTesting(void* arg){
    printStringLock("Starting worker body\n");
    int smth=0;
    for(int i=0;i<150;i++){
        for(int j=0;j<100000000;j++){
            smth=(smth+1);
        }
        smth=smth-100000000+1;
        printStringLock("Worker body testing ");
        printInt(smth);
        printStringLock("\n");
    }

}

void Threads_C_API_test() {
    thread_t threads[4];
    thread_create(&threads[0], workerBodyA, nullptr);
    printStringLock("ThreadA created\n");

    thread_create(&threads[1], workerBodyB, nullptr);
    printStringLock("ThreadB created\n");

    thread_create(&threads[2], workerBodyC, nullptr);
    printStringLock("ThreadC created\n");

    thread_create(&threads[3], workerBodyD, nullptr);
    printStringLock("ThreadD created\n");

    while (!(finishedA && finishedB && finishedC && finishedD)) {
        thread_dispatch();
    }

}



void userMain()
{

    printStringLock("Krenuo user main\n");

    Threads_C_API_test();
    // producerConsumer_C_API();
    // testSleeping();

 

    return ;
}



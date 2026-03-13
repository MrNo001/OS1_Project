//
// Created by os on 1/3/25.
//

#include "../lib/console.h"
#include "../h/MemAllocator.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.hpp"
#include "../h/print.hpp"

#include "../h/syscall_cpp.hpp"

#include "../test/ConsumerProducer_C_API_test.hpp"







static volatile bool finished1 = false;

void workerBodyA(void* args){
    print("Hello from workerBodyA\n");
    for(int i = 0; i <= 9; i++) {
        
        putc(char('h'));
        putc(char('e'));
        putc(char('j'));
        putc(char(' '));
        putc(char(i + '0'));
        putc(char('\n'));
    }
    finished1 = true;
    return;
}


int size_of_buffer = 10;
sem_t empty_slots = nullptr;
sem_t full_slots = nullptr;
int* buffer = nullptr;  // allocate in producerConsumerTest; global init runs before kernel
int read_index = 0;
int write_index = 0;

void producerBody(void* args){
    print("Hello from producerBody\n");
    while(1){
        print("Producer waiting for empty slot\n");
        sem_wait(empty_slots);
        buffer[write_index] = 1;
        write_index = (write_index + 1) % size_of_buffer;
        sem_signal(full_slots);
    }
    
}
void consumerBody(void* args){
    print("Hello from consumerBody\n");
    while(1){
        print("Consumer waiting for full slot\n");
        sem_wait(full_slots);
        int data = buffer[read_index];
        read_index = (read_index + 1) % size_of_buffer;
        putc(char(data + '0'));
        sem_signal(empty_slots);
    }
}

void producerConsumerTest(void* args){
    print("Hello from producerConsumerTest\n");

    buffer = (int*) mem_alloc(size_t(size_of_buffer * sizeof(int)));
    if (buffer == nullptr) {
        print("ProducerConsumerTest: mem_alloc buffer failed\n");
        return;
    }

    sem_open(&empty_slots, size_of_buffer);
    sem_open(&full_slots, 0);

    Thread* t_producer = new Thread(producerBody, nullptr);
    t_producer->start();
    Thread* t_consumer = new Thread(consumerBody, nullptr);
    t_consumer->start();
    time_sleep(time_t(50));
    print("ProducerConsumerTest finished\n");
    return;
}



sem_t waitTestCompletedSemaphore = nullptr;
sem_t simpleWaitTestSemaphore = nullptr;
void simpleWaitTestWorker(void* args){

    time_sleep(time_t(5));
    sem_wait(simpleWaitTestSemaphore);
    print("SimpleWaitTestWorker waited on semaphore\n");
    time_sleep(time_t(5));
    sem_signal(waitTestCompletedSemaphore);  // let main know this test is done
    return;
}

void simpleWaitTest(void* args){

    sem_open(&simpleWaitTestSemaphore, 0);
    sem_open(&waitTestCompletedSemaphore, 0);
    Thread* t_simpleWaitTest = new Thread(simpleWaitTestWorker, nullptr);
    t_simpleWaitTest->start();



    print(" Signaling simpleWaitTestSemaphore\n"); 
    sem_signal(simpleWaitTestSemaphore);
    print(" Signaled simpleWaitTestSemaphore\n");

    sem_wait(waitTestCompletedSemaphore);
    print(" SimpleWaitTest waited on semaphore\n");
}




class PeriodicThreadWorker : public PeriodicThread {
public:
    PeriodicThreadWorker(time_t period) : PeriodicThread(period) {}
    void periodicActivation() override {
        print("Hello from periodicThreadWorker\n");
        return;
    }
};


void periodicThreadTest(void* args){
    print("Hello from periodicThreadTest\n");
    PeriodicThreadWorker* t_periodicThread = new PeriodicThreadWorker(time_t(10));
    t_periodicThread->start();
    time_sleep(time_t(100));
    t_periodicThread->terminate();
    return;
}   

 void userMainTest()
{
    print("Hello from userMainTest\n");
    //simpleWaitTest(nullptr);
    //producerConsumerTest(nullptr);
    periodicThreadTest(nullptr);
    print("UserMainTest finished\n");

    return ;
}



//
// Created by os on 6/30/25.
//

#ifndef OS_PROJECT_SCHEDULER_HPP
#define OS_PROJECT_SCHEDULER_HPP

#include "../h/Queue.hpp"
#include "../h/TCB.hpp"
#include "../h/KSemaphore.hpp"
#include "../h/print.hpp"

typedef Queue<TCB*>::Node Node;

class Scheduler{
    static Queue<TCB*> queueTCB;
    static Queue<TCB*> sleeping;

    ~Scheduler(){};

    static TCB* get(){
        return queueTCB.pop();
    }
    static void put(TCB* T){
        queueTCB.put(T);
        return;
    }

    static void wakeSleeping(){
       while(1){
           TCB* thread = sleeping.peek();
           if(thread == nullptr) break;
           if(thread->wake_time<=Riscv::system_time){
               sleeping.pop();
               Scheduler::put(thread);
               continue;
           }
           break;
       }
        return;
    }

    static void sleepThread(TCB* thread,uint64 time){
        
        TCB::running->wake_time = Riscv::system_time + time;
    
        Node* current = sleeping.head;
        Node* previous = nullptr;
        while(current!= nullptr && current->data->wake_time < thread->wake_time){
            previous=current;
            current = current->next;
        }
        Node* new_node = new Node;
        new_node->data=thread;
        new_node->next=current;
        if(current == nullptr) sleeping.tail=new_node;
        if(previous!= nullptr) previous->next=new_node;
        else {sleeping.head=new_node;}

        return;
    }


    friend class TCB;
    friend class KSemaphore;
    friend class Riscv;
};

#endif //OS_PROJECT_SCHEDULER_HPP

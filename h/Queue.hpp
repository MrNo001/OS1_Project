//
// Created by os on 2/19/25.
//

#ifndef OS_PROJECT_QUEUE_HPP
#define OS_PROJECT_QUEUE_HPP

#include "../lib/hw.h"
#include "../h/MemAllocator.hpp"


template <typename T>
class Queue {

public:
    Queue();

    void put(T elem);
    T pop();
    bool pop(T& out);
    T peek();
    bool isEmpty();

    struct Node{
        Node* next;
        T data;

        void* operator new(size_t size){
            return MemoryAllocator::kmalloc(size);
        }

        void operator delete(void* ptr){
            MemoryAllocator::kfree(ptr);
        }
    };
    Node* head = nullptr;
    Node* tail = nullptr;

};

template <typename T>
Queue<T>::Queue() {
}

template <typename T>
void Queue<T>::put(T elem) {
    Node* node = new Node;
    if(node == nullptr){
        return;
    }
    node->next=nullptr;
    node->data=elem;
    if(tail)tail->next=node;
    else head=node;
    tail=node;
    return;
}

template <typename T>
T Queue<T>::pop(){
    if(head == nullptr) return nullptr;
    Node* to_pop = head;
    head = head->next;
    T ret_value = to_pop->data;
    delete to_pop;
    if(head == nullptr) tail = nullptr;
    return ret_value;
}

template <typename T>
bool Queue<T>::pop(T& out){
    if(head == nullptr) return false;
    Node* to_pop = head;
    head = head->next;
    T ret_value = to_pop->data;
    delete to_pop;
    if(head == nullptr) tail = nullptr;

    out = ret_value;
    return true;
}

template <typename T>
bool Queue<T>::isEmpty() {
    return (head == nullptr);
}

template <typename T>
T Queue<T>::peek() {
    if(head != nullptr){
        return head->data;
    }
    else{
        return nullptr;
    }
}



#endif //OS_PROJECT_QUEUE_HPP

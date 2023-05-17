#pragma once
#include "List.h"
#include <iostream>

template<class T>
class Queue : public List<T> {

public:
    void enqueue(const T& data) {
        this->pushBack(data);
    }

    T dequeue() {
        if (this->m_size > 0) {
            ListNode<T>* temp = this->m_head;
            T value = temp->data;
            this->m_head = this->m_head->next;
            if (this->m_size > 1) {
                this->m_head->before = nullptr;
            };
            delete temp;
            this->m_size--;
            return value;
        }
        else {
            throw std::exception();
        }
    }
};
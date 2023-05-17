#pragma once
#include <iostream>
#include "Vector.h"

template<typename T>
class MinHeap {
public:

    MinHeap()
    {
        m_heap.reserve(16);
    }

    MinHeap(size_t size)
    {
        m_heap.reserve(size);
    }

    void push(const T& val) {
        m_heap.pushBack(val);
        heapifyUp(m_heap.size() - 1);
    }

    T top() {
        T min_val = m_heap[0];
        m_heap[0] = m_heap.back();
        m_heap.popBack();
        heapifyDown(0);
        return min_val;
    }

    bool empty() const {
        return m_heap.empty();
    }

    size_t size() const {
        return m_heap.size();
    }

private:
    Vector<T> m_heap;

    void swap(size_t index1, size_t index2) {
        T tmp = static_cast<T&&>(m_heap[index1]);
        m_heap[index1] = static_cast<T&&>(m_heap[index2]);
        m_heap[index2] = static_cast<T&&>(tmp);
    }

    void heapifyUp(size_t index) {
        while (index > 0) {
            size_t parent = (index - 1) / 2;
            if (m_heap[parent] > m_heap[index]) {
                swap(parent, index);
                index = parent;
            }
            else {
                break;
            }
        }
    }

    void heapifyDown(size_t index) {
        while (index < m_heap.size()) {
            size_t left = index * 2 + 1;
            size_t right = index * 2 + 2;
            size_t newPos = index;
            if (left < m_heap.size() && m_heap[left] < m_heap[newPos]) {
                newPos = left;
            }
            if (right < m_heap.size() && m_heap[right] < m_heap[newPos]) {
                newPos = right;
            }
            if (newPos != index) {
                swap(newPos, index);
                index = newPos;
            }
            else {
                break;
            }
        }
    }
};
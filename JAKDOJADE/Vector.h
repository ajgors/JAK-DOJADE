#pragma once

#include <iostream>

template <typename T>
class Vector {

private:
    T* m_data;
    size_t m_size;
    size_t m_capacity;
    bool m_shrink = false;
    void            realocate(size_t new_allocated_size);

public:
    Vector();
    Vector(const Vector& other);
    Vector(Vector&& other);
    Vector(int size);
    ~Vector();
    Vector(std::initializer_list<T> initList);

    T&              operator[](size_t index);
    Vector&         operator=(const Vector& other);

    void            allocate(size_t size);
    void            pushBack(T val);
    T               popBack();
    int             size();
    int             size() const;
    void            clear();
    void            deleteAtIndex(size_t index);
    void            addToFront(T val);
    void            reserve(size_t new_allocated_size);
    size_t          getAllocatedSize();
    void            resize(size_t newSize);
    T&              front();
    T&              back();
    bool            empty() const;

    //iterators
    class Iterator;
    Iterator begin() const;
    Iterator end() const;
};


template<typename T>
Vector<T>::Vector(Vector&& other)
    : m_data(other.m_data), m_size(other.m_size), m_capacity(other.m_capacity)
{
    other.m_data = nullptr;
    other.m_size = 0;
    other.m_capacity = 0;
}

template<typename T>
Vector<T>::Vector(std::initializer_list<T> initList) {
    m_data = new T[initList.size()];
    m_capacity = initList.size();
    for (auto& i : initList) {
        pushBack(i);
    }
}

template<typename T>
bool Vector<T>::empty() const {
    return m_size == 0;
}

template<typename T>
T& Vector<T>::back() {
    return m_data[m_size - 1];
}

template<typename T>
T& Vector<T>::front() {
    return m_data[0];
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other) {
    if (this != &other) {
        T* new_data = new T[other.m_capacity];
        for (int i = 0; i < other.m_size; i++) {
            new_data[i] = other.m_data[i];
        }

        delete[] m_data;
        m_data = new_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;
    }
    return *this;
}

template<typename T>
class Vector<T>::Iterator {
private:
    T* current;
public:
    Iterator() : current(nullptr) {};
    Iterator(T* node) : current(node) {};

    const T& operator*() const {
        return *current;
    }

    T& operator*() {
        return *current;
    }

    Iterator& operator++() {
        current++;
        return *this;
    }

    Iterator& operator=(T* other) {
        this->current = other;
        return *this;
    }

    Iterator operator++(int) {
        Iterator iterator = *this;
        ++(*this);
        return iterator;
    }

    bool operator!=(const Iterator& other) {
        return current != other.current;
    }
};

template<typename T>
typename Vector<T>::Iterator Vector<T>::begin() const {
    return Iterator(m_data);
}

template<typename T>
typename Vector<T>::Iterator Vector<T>::end() const {
    return (m_data + m_size);
}

template<typename T>
Vector<T>::Vector(int size) {
    m_capacity = size;
    m_data = new T[m_capacity]{};
    m_size = size;
}

template<typename T>
size_t Vector<T>::getAllocatedSize() {
    return m_capacity;
}

template <typename T>
Vector<T>::Vector()
    : m_data(new T[2]), m_size(0), m_capacity(2)
{
    realocate(2);
}

template <typename T>
Vector<T>::Vector(const Vector& other) {
    if (this != &other) {
        m_capacity = other.m_capacity;
        m_size = other.m_size;
        T* new_data = new T[m_capacity];
        delete[] m_data;
        memcpy(new_data, other.m_data, sizeof(T) * m_size);
        m_data = new_data;
    }
}

template <typename T>
Vector<T>::~Vector() {
    delete[] m_data;
}

template <typename T>
void Vector<T>::allocate(size_t size) {
    realocate(size);
}

template <typename T>
void Vector<T>::pushBack(T val) {

    if (m_size >= m_capacity) {
        m_capacity *= 2;
        realocate(m_capacity);
    }

    m_data[m_size] = val;
    m_size++;
}

template <typename T>
T Vector<T>::popBack() {

    if (m_shrink && m_size <= m_capacity / 2) {
        m_capacity = m_capacity / 2;
        realocate(m_capacity);
    }
    if (m_size > 0) {
        m_size--;
        return m_data[m_size];
    }
    else {
        throw std::out_of_range("empty");
    }
}

template <typename T>
T& Vector<T>::operator[](size_t index) {

    if (index >= m_size) {
        throw std::out_of_range("Index out of range");
    }
    else {
        return m_data[index];
    }
}

template <typename T>
int Vector<T>::size() { return m_size; }

template <typename T>
int Vector<T>::size() const { return m_size; }

template <typename T>
void Vector<T>::clear() {

    size_t size = m_size;
    for (size_t i = 0; i < size; i++) {
        popBack();
    }
}

template <typename T>
void Vector<T>::deleteAtIndex(size_t index) {

    if (index < 0 || index >= m_size)
        return;

    if (m_shrink && m_size <= m_capacity / 2) {
        m_capacity = m_capacity / 2;
        realocate(m_capacity);
    }
    m_size--;

    for (size_t i = index; i < m_size; i++) {
        m_data[i] = m_data[i + 1];
    }
}

template <typename T>
void Vector<T>::addToFront(T val) {

    if (m_size >= m_capacity) {
        m_capacity *= 2;
        realocate(m_capacity);
    }

    for (size_t i = m_size; i > 0; i--) {
        m_data[i] = m_data[i - 1];
    }

    m_data[0] = val;
    m_size++;
}

template <typename T>
void Vector<T>::realocate(size_t new_allocated_size) {

    T* new_data = new T[new_allocated_size];
    m_capacity = new_allocated_size;
    for (size_t i = 0; i < m_size; i++) {

        new_data[i] = static_cast<T&&>(m_data[i]);
    }
    delete[] m_data;
    m_data = new_data;
}

template <typename T>
void Vector<T>::reserve(size_t new_allocated_size) {
    delete[] m_data;
    m_capacity = new_allocated_size;
    m_data = new T[m_capacity];
    m_size = 0;
}

template <typename T>
void Vector<T>::resize(size_t newSize) {
    realocate(newSize);
    m_size = newSize;
    m_capacity = newSize;
}
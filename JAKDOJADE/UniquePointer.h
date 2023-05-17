#pragma once

template<typename T>
class UniquePointer {

    T* m_ptr;

public:

    UniquePointer(T* ptr)
        : m_ptr(ptr)
    {
    }

    UniquePointer(const UniquePointer&) = delete;
    UniquePointer& operator=(const UniquePointer&) = delete;
    UniquePointer(UniquePointer&& other)
        : m_ptr(other.m_ptr)
    {
        other.m_ptr = nullptr;
    }

    UniquePointer& operator=(UniquePointer&& other)
    {
        delete m_ptr;
        m_ptr = other.m_ptr;
        other.m_ptr = nullptr;
        return *this;
    }

    ~UniquePointer()
    {
        delete m_ptr;
    }

    T* get() const
    {
        return m_ptr;
    }

    T& operator*() const
    {
        return *m_ptr;
    }
};
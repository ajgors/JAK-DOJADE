#pragma once

template<typename T, typename U>
struct Pair {
    T first;
    U second;

    Pair() : first(T()), second(U()) {}
    Pair(const T& first, const U& second) : first(first), second(second)
    {
    }

    Pair(T&& first, U&& second)
        : first(static_cast<T&&>(first)), second(static_cast<U&&>(second))
    {
    }

    bool operator<(const Pair& other) {
        return first < other.first;
    }

    bool operator>(const Pair& other) {
        return first > other.first;
    }
};
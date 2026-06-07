#ifndef DEQUELISTSEQUENCE_HPP
#define DEQUELISTSEQUENCE_HPP

#include "listsequence.hpp"
#include "exceptions.hpp"

template <typename T>
class Deque {
private:
    MutableListSequence<T> data;
public:
    void pushFront(const T& item) { data.Prepend(item); }
    void pushBack(const T& item) { data.Append(item); }
    T popFront() {
        if (data.GetLength() == 0) throw IndexOutOfRange("Deque::popFront: empty");
        T front = data.GetFirst();
        data.RemoveFirst();
        return front;
    }
    T popBack() {
        if (data.GetLength() == 0) throw IndexOutOfRange("Deque::popBack: empty");
        T back = data.GetLast();
        data.RemoveLast();
        return back;
    }
    const T& front() const { return data.GetFirst(); }
    const T& back() const { return data.GetLast(); }
    bool isEmpty() const { return data.GetLength() == 0; }
    int size() const { return data.GetLength(); }
};

#endif
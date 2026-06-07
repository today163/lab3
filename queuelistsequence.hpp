#ifndef QUEUELISTSEQUENCE_HPP
#define QUEUELISTSEQUENCE_HPP

#include "listsequence.hpp"
#include "exceptions.hpp"

template <typename T>
class Queue {
private:
    MutableListSequence<T> data;
public:
    void enqueue(const T& item) { data.Append(item); }
    T dequeue() {
        if (data.GetLength() == 0) throw IndexOutOfRange("Queue::dequeue: empty");
        T front = data.GetFirst();
        data.RemoveFirst();
        return front;
    }
    const T& front() const { return data.GetFirst(); }
    bool isEmpty() const { return data.GetLength() == 0; }
    int size() const { return data.GetLength(); }
};

#endif
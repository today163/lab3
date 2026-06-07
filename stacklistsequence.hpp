#ifndef STACKLISTSEQUENCE_HPP
#define STACKLISTSEQUENCE_HPP

#include "listsequence.hpp"
#include "exceptions.hpp"

template <typename T>
class Stack {
private:
    MutableListSequence<T> data;
public:
    void push(const T& item) { data.Append(item); }
    T pop() {
        if (data.GetLength() == 0) throw IndexOutOfRange("Stack::pop: empty");
        T top = data.GetLast();
        data.RemoveLast();
        return top;
    }
    const T& top() const { return data.GetLast(); }
    bool isEmpty() const { return data.GetLength() == 0; }
    int size() const { return data.GetLength(); }
};

#endif
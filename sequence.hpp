#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

#include "ienumerable.hpp"

template <typename T>
class Sequence : public IEnumerable<T> {
protected:
    virtual Sequence<T>* Instance() = 0;

public:
    virtual ~Sequence() = default;

    virtual const T& GetFirst() const = 0;
    virtual const T& GetLast() const = 0;
    virtual const T& Get(int index) const = 0;
    virtual int GetLength() const = 0;

    virtual Sequence<T>* GetSubsequence(int startIndex, int endIndex) const = 0;

    virtual Sequence<T>* Append(const T& item) = 0;
    virtual Sequence<T>* Prepend(const T& item) = 0;
    virtual Sequence<T>* InsertAt(const T& item, int index) = 0;
    virtual Sequence<T>* RemoveFirst() = 0;
    virtual Sequence<T>* RemoveLast() = 0;
    virtual Sequence<T>* RemoveAt(int index) = 0;

    virtual Sequence<T>* Concat(const Sequence<T>* other);

    virtual Sequence<T>* Clone() const = 0;
    virtual Sequence<T>* CreateEmpty() const = 0;

    template <typename T2>
    T2 Reduce(T2 (*f)(T2, T), T2 initial) const {
        T2 acc = initial;
        for (int i = 0; i < GetLength(); ++i) acc = f(acc, Get(i));
        return acc;
    }

    Sequence<T>* operator+(Sequence<T>* other) { return Concat(other); }

    bool operator==(const Sequence<T>& other) const {
        if (GetLength() != other.GetLength()) return false;
        auto LeftIt = this->begin();
        auto RightIt = other.begin();
        for (; (LeftIt != this->end())&&(RightIt != other.end()) ;++LeftIt, ++RightIt)
            if (*LeftIt != *RightIt) return false;
        return true;
    }

    bool IsEmpty() const { return GetLength() == 0; }
};

template <typename T>
void appendInternal(Sequence<T>*& seq, const T& item) {
    Sequence<T>* next = seq->Append(item);
    if (next != seq) { delete seq; seq = next; }
}

template <typename T>
Sequence<T>* Sequence<T>::Concat(const Sequence<T>* other) {
    Sequence<T>* inst = this->Instance();
    for (const T& item : *other)
        appendInternal(inst, item);
    return inst;
}

#endif
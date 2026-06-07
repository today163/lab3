#ifndef LIST_SEQUENCE_HPP
#define LIST_SEQUENCE_HPP

#include <iterator>
#include "sequence.hpp"
#include "linkedlist.hpp"
#include "exceptions.hpp"

template <typename T>
class ListSequence : public Sequence<T> {
protected:
    LinkedList<T> list;

    void AppendInternal(const T& item) {
        list.Append(item);
    }

    void PrependInternal(const T& item) {
        list.Prepend(item);
    }

    void InsertAtInternal(const T& item, int index) {
        list.InsertAt(item, index);
    }

    void RemoveFirstInternal() {
        list.RemoveFirst();
    }

    void RemoveLastInternal() {
        list.RemoveLast();
    }

    void RemoveAtInternal(int index) {
        list.RemoveAt(index);
    }

public:
    ListSequence() : list() {}

    ListSequence(const T* items, int count) : list(items, count) {}

    ListSequence(const ListSequence<T>& other) : list(other.list) {}

    virtual ~ListSequence() = default;

    const T& GetFirst() const override {
        if (list.GetLength() == 0)
            throw IndexOutOfRange("ListSequence::GetFirst: empty sequence");
        return list.GetFirst();
    }

    const T& GetLast() const override {
        int n = list.GetLength();
        if (n == 0)
            throw IndexOutOfRange("ListSequence::GetLast: empty sequence");
        return list.GetLast();
    }

    const T& Get(int index) const override {
        return list.Get(index);
    }

    int GetLength() const override {
        return list.GetLength();
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override;

    Sequence<T>* Append(const T& item) override {
        auto* inst = static_cast<ListSequence<T>*>(this->Instance());
        inst->AppendInternal(item);
        return inst;
    }

    Sequence<T>* Prepend(const T& item) override {
        auto* inst = static_cast<ListSequence<T>*>(this->Instance());
        inst->PrependInternal(item);
        return inst;
    }

    Sequence<T>* InsertAt(const T& item, int index) override {
        auto* inst = static_cast<ListSequence<T>*>(this->Instance());
        inst->InsertAtInternal(item, index);
        return inst;
    }

    Sequence<T>* RemoveFirst() override {
        auto* inst = static_cast<ListSequence<T>*>(this->Instance());
        inst->RemoveFirstInternal();
        return inst;
    }

    Sequence<T>* RemoveLast() override {
        auto* inst = static_cast<ListSequence<T>*>(this->Instance());
        inst->RemoveLastInternal();
        return inst;
    }

    Sequence<T>* RemoveAt(int index) override {
        auto* inst = static_cast<ListSequence<T>*>(this->Instance());
        inst->RemoveAtInternal(index);
        return inst;
    }

    T& operator[](int index) {
        return list.GetRef(index);
    }

    const T& operator[](int index) const {
        return list.Get(index);
    }

    IEnumerator<T>* GetEnumerator() const override {
        return list.GetEnumerator();
    }
};

template <typename T>
class MutableListSequence : public ListSequence<T> {
protected:
    Sequence<T>* Instance() override {
        return this;
    }

public:
    MutableListSequence() : ListSequence<T>() {}
    MutableListSequence(const T* items, int count) : ListSequence<T>(items, count) {}
    MutableListSequence(const MutableListSequence<T>& other) : ListSequence<T>(other) {}

    Sequence<T>* Clone() const override {
        return new MutableListSequence<T>(*this);
    }

    Sequence<T>* CreateEmpty() const override {
        return new MutableListSequence<T>();
    }
};

template <typename T>
class ImmutableListSequence : public ListSequence<T> {
protected:
    Sequence<T>* Instance() override {
        return this->Clone();
    }

public:
    ImmutableListSequence() : ListSequence<T>() {}
    ImmutableListSequence(const T* items, int count) : ListSequence<T>(items, count) {}
    ImmutableListSequence(const ImmutableListSequence<T>& other) : ListSequence<T>(other) {}

    Sequence<T>* Clone() const override {
        return new ImmutableListSequence<T>(*this);
    }

    Sequence<T>* CreateEmpty() const override {
        return new ImmutableListSequence<T>();
    }
};

template <typename T>
Sequence<T>* ListSequence<T>::GetSubsequence(int startIndex, int endIndex) const {
    if (startIndex < 0 || endIndex >= GetLength() || startIndex > endIndex)
        throw IndexOutOfRange("ListSequence::GetSubsequence: invalid range");

    Sequence<T>* result = this->CreateEmpty();
    for (int i = startIndex; i <= endIndex; ++i) {
        result = result->Append(Get(i));
    }
    return result;
}

#endif
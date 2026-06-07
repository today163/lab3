#ifndef ARRAY_SEQUENCE_HPP
#define ARRAY_SEQUENCE_HPP

#include "sequence.hpp"
#include "dynamicarray.hpp"
#include "exceptions.hpp"

template <typename T>
class ArraySequence : public Sequence<T> {
protected:
    DynamicArray<T> array;

    void AppendInternal(const T& item) {
        int n = array.GetSize();
        array.Resize(n + 1);
        array.Set(n, item);
    }

    void PrependInternal(const T& item) {
        int n = array.GetSize();
        array.Resize(n + 1);
        for (int i = n; i > 0; --i)
            array.Set(i, array.Get(i - 1));
        array.Set(0, item);
    }

    void InsertAtInternal(const T& item, int index) {
        int n = array.GetSize();
        if (index < 0 || index > n)
            throw IndexOutOfRange("ArraySequence::InsertAtInternal: index out of range");
        array.Resize(n + 1);
        for (int i = n; i > index; --i)
            array.Set(i, array.Get(i - 1));
        array.Set(index, item);
    }

    void RemoveFirstInternal() {
        int n = array.GetSize();
        if (n == 0)
            throw IndexOutOfRange("ArraySequence::RemoveFirstInternal: empty sequence");
        for (int i = 0; i < n - 1; ++i)
            array.Set(i, array.Get(i + 1));
        array.Resize(n - 1);
        array.ShrinkToFit();
    }

    void RemoveLastInternal() {
        int n = array.GetSize();
        if (n == 0)
            throw IndexOutOfRange("ArraySequence::RemoveLastInternal: empty sequence");
        array.Resize(n - 1);
        array.ShrinkToFit();
    }

    void RemoveAtInternal(int index) {
        int n = array.GetSize();
        if (index < 0 || index >= n)
            throw IndexOutOfRange("ArraySequence::RemoveAtInternal: index out of range");
        for (int i = index; i < n - 1; ++i)
            array.Set(i, array.Get(i + 1));
        array.Resize(n - 1);
        array.ShrinkToFit();
    }

public:
    ArraySequence() : array(DynamicArray<T>(static_cast<const T*>(nullptr), 0)) {}

    ArraySequence(const T* items, int count) : array(DynamicArray<T>(items, count)) {}

    ArraySequence(const ArraySequence<T>& other) : array(DynamicArray<T>(other.array)) {}

    virtual ~ArraySequence() = default;

    const T& GetFirst() const override {
        if (array.GetSize() == 0)
            throw IndexOutOfRange("ArraySequence::GetFirst: empty sequence");
        return array.Get(0);
    }

    const T& GetLast() const override {
        int n = array.GetSize();
        if (n == 0)
            throw IndexOutOfRange("ArraySequence::GetLast: empty sequence");
        return array.Get(n - 1);
    }

    const T& Get(int index) const override {
        return array.Get(index);
    }

    int GetLength() const override {
        return array.GetSize();
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override;

    Sequence<T>* Append(const T& item) override {
        auto* inst = static_cast<ArraySequence<T>*>(this->Instance());
        inst->AppendInternal(item);
        return inst;
    }

    Sequence<T>* Prepend(const T& item) override {
        auto* inst = static_cast<ArraySequence<T>*>(this->Instance());
        inst->PrependInternal(item);
        return inst;
    }

    Sequence<T>* InsertAt(const T& item, int index) override {
        auto* inst = static_cast<ArraySequence<T>*>(this->Instance());
        inst->InsertAtInternal(item, index);
        return inst;
    }

    Sequence<T>* RemoveFirst() override {
        auto* inst = static_cast<ArraySequence<T>*>(this->Instance());
        inst->RemoveFirstInternal();
        return inst;
    }

    Sequence<T>* RemoveLast() override {
        auto* inst = static_cast<ArraySequence<T>*>(this->Instance());
        inst->RemoveLastInternal();
        return inst;
    }

    Sequence<T>* RemoveAt(int index) override {
        auto* inst = static_cast<ArraySequence<T>*>(this->Instance());
        inst->RemoveAtInternal(index);
        return inst;
    }

    T& operator[](int index) {
        return array[index];
    }

    const T& operator[](int index) const {
        return array[index];
    }

    IEnumerator<T>* GetEnumerator() const override {
        return array.GetEnumerator();
    }
};

template <typename T>
class MutableArraySequence : public ArraySequence<T> {
protected:
    Sequence<T>* Instance() override {
        return this;
    }

public:
    MutableArraySequence() : ArraySequence<T>() {}
    MutableArraySequence(const T* items, int count) : ArraySequence<T>(items, count) {}
    MutableArraySequence(const MutableArraySequence<T>& other) : ArraySequence<T>(other) {}

    Sequence<T>* Clone() const override {
        return new MutableArraySequence<T>(*this);
    }

    Sequence<T>* CreateEmpty() const override {
        return new MutableArraySequence<T>();
    }
};

template <typename T>
class ImmutableArraySequence : public ArraySequence<T> {
protected:
    Sequence<T>* Instance() override {
        return this->Clone();
    }

public:
    ImmutableArraySequence() : ArraySequence<T>() {}
    ImmutableArraySequence(const T* items, int count) : ArraySequence<T>(items, count) {}
    ImmutableArraySequence(const ImmutableArraySequence<T>& other) : ArraySequence<T>(other) {}

    Sequence<T>* Clone() const override {
        return new ImmutableArraySequence<T>(*this);
    }

    Sequence<T>* CreateEmpty() const override {
        return new ImmutableArraySequence<T>();
    }

};

template <typename T>
Sequence<T>* ArraySequence<T>::GetSubsequence(int startIndex, int endIndex) const {
    if (startIndex < 0 || endIndex >= GetLength() || startIndex > endIndex)
        throw IndexOutOfRange("ArraySequence::GetSubsequence: invalid range");

    Sequence<T>* result = this->CreateEmpty();
    for (int i = startIndex; i <= endIndex; ++i) {
        result = result->Append(Get(i));
    }
    return result;
}

#endif
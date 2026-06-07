#ifndef DYNAMICARRAY_HPP
#define DYNAMICARRAY_HPP

#include "exceptions.hpp"
#include "ienumerable.hpp"

template <typename T>
class DynamicArray : public IEnumerable<T> {
    T* data;
    int size;
    int capacity;

    static constexpr int INITIAL_CAPACITY = 8;

    static int calcCapacity(int size) {
        int cap = INITIAL_CAPACITY;
        while (cap < size) cap *= 2;
        return cap;
    }

    void reallocate(int newCapacity) {
        T* nd = new T[newCapacity]();
        for (int i = 0; i < size; ++i) nd[i] = data[i];
        delete[] data;
        data = nd;
        capacity = newCapacity;
    }

public:
    DynamicArray(const T* items, int count) : size(count), capacity(calcCapacity(count)) {
        if (count < 0) throw "DynamicArray: negative count";
        data = new T[capacity];
        if (items != nullptr)
            for (int i = 0; i < count; ++i) data[i] = items[i];
        else
            for (int i = 0; i < count; ++i) data[i] = T();
    }

    DynamicArray(const DynamicArray<T>& other) : size(other.size), capacity(other.capacity) {
        data = new T[capacity];
        for (int i = 0; i < size; ++i) data[i] = other.data[i];
    }

    DynamicArray& operator=(const DynamicArray<T>& other) {
        if (this == &other) return *this;
        delete[] data;
        size = other.size;
        capacity = other.capacity;
        data = new T[capacity];
        for (int i = 0; i < size; ++i) data[i] = other.data[i];
        return *this;
    }

    ~DynamicArray() { delete[] data; }

    const T& Get(int index) const {
        if (index < 0 || index >= size)
            throw IndexOutOfRange("DynamicArray::Get: index out of range");
        return data[index];
    }

    int GetSize() const { return size; }

    void Set(int index, const T& value) {
        if (index < 0 || index >= size)
            throw IndexOutOfRange("DynamicArray::Set: index out of range");
        data[index] = value;
    }

    void Resize(int newSize) {
        if (newSize < 0)
            throw "DynamicArray::Resize: negative size";
        if (newSize > capacity) {
            int newCapacity = capacity;
            while (newCapacity < newSize)
                newCapacity *= 2;
            reallocate(newCapacity);
        }
        size = newSize;
    }

    void ShrinkToFit() {
        int newCapacity = capacity;
        while (newCapacity / 2 >= size && newCapacity / 2 >= INITIAL_CAPACITY)
            newCapacity /= 2;
        if (newCapacity != capacity)
            reallocate(newCapacity);
    }

    T& operator[](int index) {
        if (index < 0 || index >= size)
            throw IndexOutOfRange("DynamicArray::operator[]: index out of range");
        return data[index];
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= size)
            throw IndexOutOfRange("DynamicArray::operator[]: index out of range");
        return data[index];
    }

    class Enumerator : public IEnumerator<T> {
        const DynamicArray<T>* arr;
        int pos;
    public:
        explicit Enumerator(const DynamicArray<T>* a) : arr(a), pos(-1) {}
        bool MoveNext() override { return ++pos < arr->size; }
        T Current()  const override {
            if (pos < 0 || pos >= arr->size)
                throw IndexOutOfRange("DynamicArray::Enumerator::Current: index out of range");
            return arr->data[pos];
        }
        void Reset() override { pos = -1; }
    };

    IEnumerator<T>* GetEnumerator() const override { return new Enumerator(this); }
};

#endif
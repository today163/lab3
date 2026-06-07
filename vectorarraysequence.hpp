#ifndef VECTORARRAYSEQUENCE_HPP
#define VECTORARRAYSEQUENCE_HPP

#include "arraysequence.hpp"
#include "exceptions.hpp"

template <typename T>
class Vector {
private:
    ImmutableArraySequence<T> data;

public:
    Vector() : data() {}

    Vector(int count) : data(nullptr, count) {}

    Vector(const T* items, int count) : data(items, count) {}

    Vector(const Vector<T>& other) : data(other.data) {}

    T& operator[](int index) {
        return data[index];
    }

    const T& operator[](int index) const {
        return data[index];
    }

    int GetLength() const {
        return data.GetLength();
    }

    Vector<T> operator+(const Vector<T>& other) const {
        int len = GetLength();
        if (len != other.GetLength())
            throw IndexOutOfRange("Vector::operator+: different lengths");

        T* temp = new T[len];
        auto leftIt = data.begin();
        auto rightIt = other.data.begin();
        for (int i = 0; (leftIt != data.end()) && (rightIt != other.data.end()); ++leftIt, ++rightIt, ++i) {
            temp[i] = *leftIt + *rightIt;
        }
        Vector<T> result(temp, len);
        delete[] temp;
        return result;
    }

    Vector<T> operator-(const Vector<T>& other) const {
        int len = GetLength();
        if (len != other.GetLength())
            throw IndexOutOfRange("Vector::operator-: different lengths");

        T* temp = new T[len];
        auto leftIt = data.begin();
        auto rightIt = other.data.begin();
        for (int i = 0; (leftIt != data.end()) && (rightIt != other.data.end()); ++leftIt, ++rightIt, ++i) {
            temp[i] = *leftIt - *rightIt;
        }
        Vector<T> result(temp, len);
        delete[] temp;
        return result;
    }

    bool operator==(const Vector<T>& other) const {
        if (GetLength() != other.GetLength()) return false;
        auto leftIt = data.begin();
        auto rightIt = other.data.begin();
        for (; (leftIt != data.end()) && (rightIt != other.data.end()); ++leftIt, ++rightIt)
            if (*leftIt != *rightIt) return false;
        return true;
    }

    bool operator!=(const Vector<T>& other) const {
        return !(*this == other);
    }

    bool operator<(const Vector<T>& other) const {
        auto leftIt = data.begin();
        auto rightIt = other.data.begin();
        while (leftIt != data.end() && rightIt != other.data.end()) {
            if (*leftIt != *rightIt)
                return *leftIt < *rightIt;
            ++leftIt;
            ++rightIt;
        }
        if (leftIt != data.end()) return false;
        if (rightIt != other.data.end()) return true;
        return false;
    }

    bool operator>(const Vector<T>& other) const {
        return other < *this;
    }

    bool operator<=(const Vector<T>& other) const {
        return !(other < *this);
    }

    bool operator>=(const Vector<T>& other) const {
        return !(*this < other);
    }

    auto begin() const { return data.begin(); }
    auto end() const { return data.end(); }
};

#endif
#ifndef IENUMERABLE_HPP
#define IENUMERABLE_HPP

template <typename T>
class IEnumerator {
public:
    virtual ~IEnumerator() = default;
    virtual bool MoveNext() = 0;
    virtual T Current() const = 0;
    virtual void Reset() = 0;
};

template <typename T>
class EnumeratorAdapter {
    IEnumerator<T>* enumerator;
    bool valid;

public:
    explicit EnumeratorAdapter(IEnumerator<T>* e) : enumerator(e), valid(e != nullptr && e->MoveNext()) {}

    EnumeratorAdapter() : enumerator(nullptr), valid(false) {}

    ~EnumeratorAdapter() { delete enumerator; }

    EnumeratorAdapter(const EnumeratorAdapter&) = delete;
    EnumeratorAdapter& operator=(const EnumeratorAdapter&) = delete;

    EnumeratorAdapter(EnumeratorAdapter&& o) noexcept : enumerator(o.enumerator), valid(o.valid) {
        o.enumerator = nullptr;
        o.valid = false;
    }

    EnumeratorAdapter& operator++() {
        if (enumerator) valid = enumerator->MoveNext();
        return *this;
    }

    T operator*() const { return enumerator->Current(); }
    bool operator!=(const EnumeratorAdapter& o) const { return valid != o.valid; }
};

template <typename T>
class IEnumerable {
public:
    virtual ~IEnumerable() = default;
    virtual IEnumerator<T>* GetEnumerator() const = 0;

    EnumeratorAdapter<T> begin() const { 
        return EnumeratorAdapter<T>(GetEnumerator()); 
    }

    EnumeratorAdapter<T> end() const { 
        return EnumeratorAdapter<T>(); 
    }
};

#endif 
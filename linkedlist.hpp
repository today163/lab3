#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <string>
#include "exceptions.hpp"
#include "ienumerable.hpp"

template <typename T>
class LinkedList : public IEnumerable<T> {
    struct Node {
        T data;
        Node* next;
        explicit Node(const T& d) : data(d), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int length;

    void Clear() {
        Node* cur = head;
        while (cur) { Node* n = cur->next; delete cur; cur = n; }
        head = tail = nullptr;
        length = 0;
    }

public:
    LinkedList(const T* items, int count) : head(nullptr), tail(nullptr), length(0) {
        for (int i = 0; i < count; ++i) Append(items[i]);
    }

    LinkedList() : head(nullptr), tail(nullptr), length(0) {}

    LinkedList(const LinkedList<T>& other) : head(nullptr), tail(nullptr), length(0) {
        for (Node* cur = other.head; cur; cur = cur->next) Append(cur->data);
    }

    LinkedList& operator=(const LinkedList<T>& other) {
        if (this == &other) return *this;
        Clear();
        for (Node* cur = other.head; cur; cur = cur->next) Append(cur->data);
        return *this;
    }

    ~LinkedList() { Clear(); }

    const T& GetFirst() const {
        if (!head) throw IndexOutOfRange("LinkedList::GetFirst: list is empty");
        return head->data;
    }

    const T& GetLast() const {
        if (!tail) throw IndexOutOfRange("LinkedList::GetLast: list is empty");
        return tail->data;
    }

    const T& Get(int index) const {
        if (index < 0 || index >= length)
            throw IndexOutOfRange("LinkedList::Get: Index out of range");
        Node* cur = head;
        for (int i = 0; i < index; ++i) cur = cur->next;
        return cur->data;
    }

    T& GetRef(int index) {
        if (index < 0 || index >= length)
            throw IndexOutOfRange("LinkedList::GetRef: Index out of range");
        Node* cur = head;
        for (int i = 0; i < index; ++i) cur = cur->next;
        return cur->data;
    }

    LinkedList<T>* GetSubList(int startIndex, int endIndex) const {
        if (startIndex < 0 || endIndex >= length || startIndex > endIndex)
            throw IndexOutOfRange("LinkedList::GetSubList: Index out of range");
        auto* result = new LinkedList<T>();
        Node* cur = head;
        for (int i = 0; i < startIndex; ++i) cur = cur->next;
        for (int i = startIndex; i <= endIndex; ++i, cur = cur->next)
            result->Append(cur->data);
        return result;
    }

    int GetLength() const { return length; }

    void Append(const T& item) {
        Node* node = new Node(item);
        if (!tail) { head = tail = node; }
        else { tail->next = node; tail = node; }
        ++length;
    }

    void Prepend(const T& item) {
        Node* node = new Node(item);
        node->next = head;
        head = node;
        if (!tail) tail = head;
        ++length;
    }

    void InsertAt(const T& item, int index) {
        if (index < 0 || index > length)
            throw IndexOutOfRange("LinkedList::InsertAt: Index out of range");
        if (index == 0) { Prepend(item); return; }
        if (index == length) { Append(item);  return; }
        Node* prev = head;
        for (int i = 0; i < index - 1; ++i) prev = prev->next;
        Node* node = new Node(item);
        node->next = prev->next;
        prev->next = node;
        ++length;
    }

    void RemoveFirst() {
        if (!head) throw IndexOutOfRange("LinkedList::RemoveFirst: list is empty");
        Node* toDelete = head;
        head = head->next;
        if (!head) tail = nullptr;
        delete toDelete;
        --length;
    }

    void RemoveLast() {
        if (!tail) throw IndexOutOfRange("LinkedList::RemoveLast: list is empty");
        if (head == tail) {
            delete head;
            head = tail = nullptr;
            --length;
            return;
        }
        Node* prev = head;
        while (prev->next != tail) prev = prev->next;
        delete tail;
        tail = prev;
        tail->next = nullptr;
        --length;
    }

    void RemoveAt(int index) {
        if (index < 0 || index >= length)
            throw IndexOutOfRange("LinkedList::RemoveAt: Index out of range");
        if (index == 0) { RemoveFirst(); return; }
        if (index == length - 1) { RemoveLast();  return; }
        Node* prev = head;
        for (int i = 0; i < index - 1; ++i) prev = prev->next;
        Node* toDelete = prev->next;
        prev->next = toDelete->next;
        delete toDelete;
        --length;
    }

    LinkedList<T>* Concat(const LinkedList<T>* other) const {
        auto* result = new LinkedList<T>(*this);
        for (Node* cur = other->head; cur; cur = cur->next)
            result->Append(cur->data);
        return result;
    }

        class Enumerator : public IEnumerator<T> {
        const LinkedList<T>* list;
        const Node* current;
        int pos;
    public:
        explicit Enumerator(const LinkedList<T>* l) : list(l), current(nullptr), pos(-1) {}

        bool MoveNext() override {
            if (pos == -1) {
                current = list->head;
                pos = 0;
            } else {
                if (current) current = current->next;
                ++pos;
            }
            return current != nullptr;
        }

        T Current() const override {
            if (!current || pos < 0 || pos >= list->GetLength())
                throw IndexOutOfRange("LinkedList::Enumerator::Current: invalid position");
            return current->data;
        }

        void Reset() override {
            current = nullptr;
            pos = -1;
        }
    };

    IEnumerator<T>* GetEnumerator() const override {
        return new Enumerator(this);
    }
};

#endif
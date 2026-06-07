#include <gtest/gtest.h>
#include "dequelistsequence.hpp"
#include "queuelistsequence.hpp"
#include "stacklistsequence.hpp"
#include "listsequence.hpp"
#include "arraysequence.hpp"

TEST(Stack, PushAndTop) {
    Stack<int> s;
    s.push(10);
    EXPECT_EQ(s.top(), 10);
    EXPECT_EQ(s.size(), 1);
    EXPECT_FALSE(s.isEmpty());
}

TEST(Stack, Pop) {
    Stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);
    EXPECT_EQ(s.pop(), 3);
    EXPECT_EQ(s.pop(), 2);
    EXPECT_EQ(s.pop(), 1);
    EXPECT_TRUE(s.isEmpty());
}

TEST(Stack, PopOnEmpty) {
    Stack<int> s;
    EXPECT_THROW(s.pop(), IndexOutOfRange);
}

TEST(Stack, TopOnEmpty) {
    Stack<int> s;
    EXPECT_THROW(s.top(), IndexOutOfRange);
}

TEST(Stack, SizeAfterMultipleOps) {
    Stack<int> s;
    EXPECT_EQ(s.size(), 0);
    s.push(5);
    s.push(7);
    EXPECT_EQ(s.size(), 2);
    s.pop();
    EXPECT_EQ(s.size(), 1);
    s.pop();
    EXPECT_EQ(s.size(), 0);
}

TEST(Stack, ConstTop) {
    Stack<int> s;
    s.push(42);
    const Stack<int>& ref = s;
    EXPECT_EQ(ref.top(), 42);
}

TEST(Queue, EnqueueAndFront) {
    Queue<int> q;
    q.enqueue(10);
    EXPECT_EQ(q.front(), 10);
    EXPECT_EQ(q.size(), 1);
    EXPECT_FALSE(q.isEmpty());
}

TEST(Queue, DequeueOrder) {
    Queue<int> q;
    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);
    EXPECT_EQ(q.dequeue(), 1);
    EXPECT_EQ(q.dequeue(), 2);
    EXPECT_EQ(q.dequeue(), 3);
    EXPECT_TRUE(q.isEmpty());
}

TEST(Queue, DequeueOnEmpty) {
    Queue<int> q;
    EXPECT_THROW(q.dequeue(), IndexOutOfRange);
}

TEST(Queue, FrontOnEmpty) {
    Queue<int> q;
    EXPECT_THROW(q.front(), IndexOutOfRange);
}

TEST(Queue, SizeAfterMixedOps) {
    Queue<int> q;
    EXPECT_EQ(q.size(), 0);
    q.enqueue(5);
    q.enqueue(6);
    EXPECT_EQ(q.size(), 2);
    q.dequeue();
    EXPECT_EQ(q.size(), 1);
    q.dequeue();
    EXPECT_EQ(q.size(), 0);
}

TEST(Queue, ConstFront) {
    Queue<int> q;
    q.enqueue(99);
    const Queue<int>& ref = q;
    EXPECT_EQ(ref.front(), 99);
}

TEST(Deque, PushFrontAndFront) {
    Deque<int> d;
    d.pushFront(10);
    EXPECT_EQ(d.front(), 10);
    EXPECT_EQ(d.back(), 10);
    EXPECT_EQ(d.size(), 1);
}

TEST(Deque, PushBackAndBack) {
    Deque<int> d;
    d.pushBack(20);
    EXPECT_EQ(d.back(), 20);
    EXPECT_EQ(d.front(), 20);
}

TEST(Deque, PushBothSides) {
    Deque<int> d;
    d.pushBack(1);
    d.pushFront(0);
    d.pushBack(2);
    EXPECT_EQ(d.front(), 0);
    EXPECT_EQ(d.back(), 2);
    EXPECT_EQ(d.size(), 3);
}

TEST(Deque, PopFront) {
    Deque<int> d;
    d.pushBack(10);
    d.pushBack(20);
    d.pushFront(5);
    EXPECT_EQ(d.popFront(), 5);
    EXPECT_EQ(d.front(), 10);
    EXPECT_EQ(d.size(), 2);
}

TEST(Deque, PopBack) {
    Deque<int> d;
    d.pushBack(10);
    d.pushBack(20);
    d.pushFront(5);
    EXPECT_EQ(d.popBack(), 20);
    EXPECT_EQ(d.back(), 10);
    EXPECT_EQ(d.size(), 2);
}

TEST(Deque, PopFrontOnEmpty) {
    Deque<int> d;
    EXPECT_THROW(d.popFront(), IndexOutOfRange);
}

TEST(Deque, PopBackOnEmpty) {
    Deque<int> d;
    EXPECT_THROW(d.popBack(), IndexOutOfRange);
}

TEST(Deque, FrontOnEmpty) {
    Deque<int> d;
    EXPECT_THROW(d.front(), IndexOutOfRange);
}

TEST(Deque, BackOnEmpty) {
    Deque<int> d;
    EXPECT_THROW(d.back(), IndexOutOfRange);
}

TEST(Deque, MixedOrder) {
    Deque<int> d;
    d.pushBack(1);
    d.pushBack(2);
    d.pushFront(0);
    EXPECT_EQ(d.popFront(), 0);
    EXPECT_EQ(d.popBack(), 2);
    EXPECT_EQ(d.popFront(), 1);
    EXPECT_TRUE(d.isEmpty());
}

TEST(Deque, SizeAfterOps) {
    Deque<int> d;
    EXPECT_EQ(d.size(), 0);
    d.pushFront(5);
    d.pushBack(6);
    EXPECT_EQ(d.size(), 2);
    d.popFront();
    EXPECT_EQ(d.size(), 1);
    d.popBack();
    EXPECT_EQ(d.size(), 0);
}

TEST(Deque, ConstFrontBack) {
    Deque<int> d;
    d.pushBack(7);
    d.pushFront(3);
    const Deque<int>& ref = d;
    EXPECT_EQ(ref.front(), 3);
    EXPECT_EQ(ref.back(), 7);
}

#include <gtest/gtest.h>
#include "vectorarraysequence.hpp"

TEST(Vector, DefaultConstructor) {
    Vector<int> v;
    EXPECT_EQ(v.GetLength(), 0);
}

TEST(Vector, ConstructorFromArray) {
    int arr[] = {1, 2, 3, 4};
    Vector<int> v(arr, 4);
    EXPECT_EQ(v.GetLength(), 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);
}

TEST(Vector, CopyConstructor) {
    int arr[] = {5, 6, 7};
    Vector<int> v1(arr, 3);
    Vector<int> v2(v1);
    EXPECT_EQ(v2.GetLength(), 3);
    EXPECT_EQ(v2[0], 5);
    EXPECT_EQ(v2[1], 6);
    EXPECT_EQ(v2[2], 7);
}

TEST(Vector, IndexOperatorValid) {
    int arr[] = {10, 20, 30};
    Vector<int> v(arr, 3);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 30);
}

TEST(Vector, IndexOperatorOutOfRange) {
    int arr[] = {1, 2};
    Vector<int> v(arr, 2);
    EXPECT_THROW(v[-1], IndexOutOfRange);
    EXPECT_THROW(v[2], IndexOutOfRange);
}

TEST(Vector, GetLength) {
    Vector<double> v1;
    EXPECT_EQ(v1.GetLength(), 0);
    double arr[] = {1.1, 2.2};
    Vector<double> v2(arr, 2);
    EXPECT_EQ(v2.GetLength(), 2);
}

TEST(Vector, OperatorPlusSameLength) {
    int a1[] = {1, 2, 3};
    int a2[] = {4, 5, 6};
    Vector<int> v1(a1, 3);
    Vector<int> v2(a2, 3);
    Vector<int> v3 = v1 + v2;
    EXPECT_EQ(v3.GetLength(), 3);
    EXPECT_EQ(v3[0], 5);
    EXPECT_EQ(v3[1], 7);
    EXPECT_EQ(v3[2], 9);
}

TEST(Vector, OperatorPlusDifferentLength) {
    int a1[] = {1, 2};
    int a2[] = {3, 4, 5};
    Vector<int> v1(a1, 2);
    Vector<int> v2(a2, 3);
    EXPECT_THROW(v1 + v2, IndexOutOfRange);
    EXPECT_THROW(v2 + v1, IndexOutOfRange);
}

TEST(Vector, OperatorMinusSameLength) {
    int a1[] = {5, 7, 9};
    int a2[] = {1, 2, 3};
    Vector<int> v1(a1, 3);
    Vector<int> v2(a2, 3);
    Vector<int> v3 = v1 - v2;
    EXPECT_EQ(v3.GetLength(), 3);
    EXPECT_EQ(v3[0], 4);
    EXPECT_EQ(v3[1], 5);
    EXPECT_EQ(v3[2], 6);
}

TEST(Vector, OperatorMinusDifferentLength) {
    int a1[] = {10, 20};
    int a2[] = {5, 5, 5};
    Vector<int> v1(a1, 2);
    Vector<int> v2(a2, 3);
    EXPECT_THROW(v1 - v2, IndexOutOfRange);
}

TEST(Vector, Equality) {
    int a1[] = {1, 2, 3};
    int a2[] = {1, 2, 3};
    int a3[] = {1, 2, 4};
    Vector<int> v1(a1, 3);
    Vector<int> v2(a2, 3);
    Vector<int> v3(a3, 3);
    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 == v3);
}

TEST(Vector, Inequality) {
    int a1[] = {1, 2, 3};
    int a2[] = {1, 2, 4};
    Vector<int> v1(a1, 3);
    Vector<int> v2(a2, 3);
    EXPECT_TRUE(v1 != v2);
    EXPECT_FALSE(v1 != v1);
}

TEST(Vector, LessThan) {
    int a1[] = {1, 2, 3};
    int a2[] = {1, 2, 4};
    int a3[] = {1, 2};
    Vector<int> v1(a1, 3);
    Vector<int> v2(a2, 3);
    Vector<int> v3(a3, 2);
    EXPECT_TRUE(v1 < v2);
    EXPECT_FALSE(v2 < v1);
    EXPECT_FALSE(v1 < v3);
    EXPECT_TRUE(v3 < v1);
}

TEST(Vector, GreaterThan) {
    int a1[] = {5, 6};
    int a2[] = {4, 7};
    Vector<int> v1(a1, 2);
    Vector<int> v2(a2, 2);
    EXPECT_TRUE(v1 > v2);
    EXPECT_FALSE(v2 > v1);
}

TEST(Vector, LessOrEqual) {
    int a1[] = {1, 2};
    int a2[] = {1, 2};
    int a3[] = {1, 3};
    Vector<int> v1(a1, 2);
    Vector<int> v2(a2, 2);
    Vector<int> v3(a3, 2);
    EXPECT_TRUE(v1 <= v2);
    EXPECT_TRUE(v1 <= v3);
    EXPECT_FALSE(v3 <= v1);
}

TEST(Vector, GreaterOrEqual) {
    int a1[] = {3, 3};
    int a2[] = {2, 4};
    Vector<int> v1(a1, 2);
    Vector<int> v2(a2, 2);
    EXPECT_TRUE(v1 >= v2);
    EXPECT_TRUE(v1 >= v1);
    EXPECT_FALSE(v2 >= v1);
}

TEST(Vector, RangeBasedForLoop) {
    int arr[] = {10, 20, 30};
    Vector<int> v(arr, 3);
    int sum = 0;
    for (int x : v) {
        sum += x;
    }
    EXPECT_EQ(sum, 60);
}

TEST(Vector, ConstCorrectness) {
    int arr[] = {1, 2, 3};
    const Vector<int> v(arr, 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v.GetLength(), 3);
}

TEST(Vector, EmptyVectorComparisons) {
    Vector<int> v1;
    Vector<int> v2;
    int arr[] = {1};
    Vector<int> v3(arr, 1);
    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 == v3);
    EXPECT_TRUE(v1 < v3);
    EXPECT_FALSE(v3 < v1);
    EXPECT_TRUE(v1 <= v2);
    EXPECT_TRUE(v1 <= v3);
}

TEST(Vector, SquareBracketsVector) {
    Vector<int> v(3);
    v[0] = 10;
    v[1] = 20;
    v[2] = 30;
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 30);
}
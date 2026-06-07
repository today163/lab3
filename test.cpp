#include <gtest/gtest.h>
#include "dynamicarray.hpp"
#include "linkedlist.hpp"
#include "arraysequence.hpp"
#include "listsequence.hpp"

TEST(DynamicArray, ConstructFromArray) {
    int arr[] = {1, 2, 3};
    DynamicArray<int> da(arr, 3);
    EXPECT_EQ(da.GetSize(), 3);
    EXPECT_EQ(da.Get(0), 1);
    EXPECT_EQ(da.Get(2), 3);
}

TEST(DynamicArray, ConstructBySize) {
    int dummy[5] = {0, 0, 0, 0, 0};
    DynamicArray<int> da(dummy, 5);
    EXPECT_EQ(da.GetSize(), 5);
}

TEST(DynamicArray, CopyConstructor) {
    int arr[] = {10, 20};
    DynamicArray<int> original(arr, 2);
    DynamicArray<int> copy(original);
    copy.Set(0, 99);
    EXPECT_EQ(original.Get(0), 10);
    EXPECT_EQ(copy.Get(0), 99);
}

TEST(DynamicArray, SetAndGet) {
    int dummy[3] = {0, 0, 0};
    DynamicArray<int> da(dummy, 3);
    da.Set(1, 42);
    EXPECT_EQ(da.Get(1), 42);
}

TEST(DynamicArray, ResizeGrow) {
    int arr[] = {1, 2, 3};
    DynamicArray<int> da(arr, 3);
    da.Resize(5);
    EXPECT_EQ(da.GetSize(), 5);
    EXPECT_EQ(da.Get(0), 1);
    EXPECT_EQ(da.Get(2), 3);
}

TEST(DynamicArray, ResizeShrink) {
    int arr[] = {1, 2, 3, 4, 5};
    DynamicArray<int> da(arr, 5);
    da.Resize(3);
    EXPECT_EQ(da.GetSize(), 3);
    EXPECT_EQ(da.Get(2), 3);
}

TEST(DynamicArray, ZeroSize) {
    int dummy[0] = {};
    DynamicArray<int> da(dummy, 0);
    EXPECT_EQ(da.GetSize(), 0);
}

TEST(DynamicArray, SingleElement) {
    int arr[] = {7};
    DynamicArray<int> da(arr, 1);
    EXPECT_EQ(da.Get(0), 7);
}

TEST(DynamicArray, GetOutOfRange) {
    int dummy[3] = {0, 0, 0};
    DynamicArray<int> da(dummy, 3);
    EXPECT_THROW(da.Get(-1), IndexOutOfRange);
    EXPECT_THROW(da.Get(3), IndexOutOfRange);
}

TEST(DynamicArray, SetOutOfRange) {
    int dummy[3] = {0, 0, 0};
    DynamicArray<int> da(dummy, 3);
    EXPECT_THROW(da.Set(-1, 0), IndexOutOfRange);
    EXPECT_THROW(da.Set(3, 0), IndexOutOfRange);
}

TEST(DynamicArray, NegativeSize) {
    int arr[] = {1};
    EXPECT_ANY_THROW(DynamicArray<int> da(arr, -1));
}

//linkedlist

TEST(LinkedList, ConstructFromArray) {
    int arr[] = {5, 6, 7};
    LinkedList<int> ll(arr, 3);
    EXPECT_EQ(ll.GetLength(), 3);
    EXPECT_EQ(ll.GetFirst(), 5);
    EXPECT_EQ(ll.GetLast(), 7);
    EXPECT_EQ(ll.Get(1), 6);
}

TEST(LinkedList, EmptyList) {
    LinkedList<int> ll;
    EXPECT_EQ(ll.GetLength(), 0);
    EXPECT_THROW(ll.GetFirst(), IndexOutOfRange);
    EXPECT_THROW(ll.GetLast(), IndexOutOfRange);
}

TEST(LinkedList, CopyConstructor) {
    int arr[] = {1, 2, 3};
    LinkedList<int> ll(arr, 3);
    LinkedList<int> copy(ll);
    copy.Prepend(0);
    EXPECT_EQ(ll.GetLength(), 3);
    EXPECT_EQ(copy.GetFirst(), 0);
}

TEST(LinkedList, AppendPrepend) {
    LinkedList<int> ll;
    ll.Append(10);
    ll.Append(20);
    ll.Prepend(5);
    EXPECT_EQ(ll.GetFirst(), 5);
    EXPECT_EQ(ll.GetLast(), 20);
    EXPECT_EQ(ll.GetLength(), 3);
}

TEST(LinkedList, InsertAt) {
    int arr[] = {1, 3};
    LinkedList<int> ll(arr, 2);
    ll.InsertAt(2, 1);
    EXPECT_EQ(ll.Get(1), 2);
    EXPECT_EQ(ll.GetLength(), 3);
}

TEST(LinkedList, GetSubList) {
    int arr[] = {0, 1, 2, 3, 4};
    LinkedList<int> ll(arr, 5);
    auto* sub = ll.GetSubList(1, 3);
    EXPECT_EQ(sub->GetLength(), 3);
    EXPECT_EQ(sub->Get(0), 1);
    EXPECT_EQ(sub->Get(2), 3);
    delete sub;
}

TEST(LinkedList, Concat) {
    int a[] = {1, 2}, b[] = {3, 4};
    LinkedList<int> la(a, 2), lb(b, 2);
    auto* cat = la.Concat(&lb);
    EXPECT_EQ(cat->GetLength(), 4);
    EXPECT_EQ(cat->Get(3), 4);
    delete cat;
}

TEST(LinkedList, OutOfRange) {
    int arr[] = {1, 2};
    LinkedList<int> ll(arr, 2);
    EXPECT_THROW(ll.Get(-1), IndexOutOfRange);
    EXPECT_THROW(ll.Get(2), IndexOutOfRange);
    EXPECT_THROW(ll.InsertAt(0, -1), IndexOutOfRange);
    EXPECT_THROW(ll.InsertAt(0, 3), IndexOutOfRange);
}

TEST(LinkedList, RemoveFirst) {
    int arr[] = {1, 2, 3};
    LinkedList<int> ll(arr, 3);
    ll.RemoveFirst();
    EXPECT_EQ(ll.GetFirst(), 2);
    EXPECT_EQ(ll.GetLength(), 2);
}

TEST(LinkedList, RemoveLast) {
    int arr[] = {1, 2, 3};
    LinkedList<int> ll(arr, 3);
    ll.RemoveLast();
    EXPECT_EQ(ll.GetLast(), 2);
    EXPECT_EQ(ll.GetLength(), 2);
}

TEST(LinkedList, RemoveAt) {
    int arr[] = {1, 2, 3};
    LinkedList<int> ll(arr, 3);
    ll.RemoveAt(1);
    EXPECT_EQ(ll.Get(1), 3);
    EXPECT_EQ(ll.GetLength(), 2);
}

TEST(LinkedList, RemoveSingleElement) {
    int arr[] = {42};
    LinkedList<int> ll(arr, 1);
    ll.RemoveFirst();
    EXPECT_EQ(ll.GetLength(), 0);
    EXPECT_THROW(ll.GetFirst(), IndexOutOfRange);
}

TEST(LinkedList, RemoveFirstEmpty) {
    LinkedList<int> ll;
    EXPECT_THROW(ll.RemoveFirst(), IndexOutOfRange);
}

TEST(LinkedList, RemoveLastEmpty) {
    LinkedList<int> ll;
    EXPECT_THROW(ll.RemoveLast(), IndexOutOfRange);
}

TEST(LinkedList, RemoveAtOutOfRange) {
    int arr[] = {1, 2};
    LinkedList<int> ll(arr, 2);
    EXPECT_THROW(ll.RemoveAt(-1), IndexOutOfRange);
    EXPECT_THROW(ll.RemoveAt(2), IndexOutOfRange);
} 

TEST(MutableArraySequence, DefaultConstructor) {
    MutableArraySequence<int> seq;
    EXPECT_EQ(seq.GetLength(), 0);
    EXPECT_THROW(seq.GetFirst(), IndexOutOfRange);
    EXPECT_THROW(seq.GetLast(), IndexOutOfRange);
}

TEST(MutableArraySequence, ConstructFromArray) {
    int arr[] = {10, 20, 30};
    MutableArraySequence<int> seq(arr, 3);
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.GetFirst(), 10);
    EXPECT_EQ(seq.GetLast(), 30);
    EXPECT_EQ(seq.Get(1), 20);
}

TEST(MutableArraySequence, CopyConstructor) {
    int arr[] = {1, 2, 3};
    MutableArraySequence<int> original(arr, 3);
    MutableArraySequence<int> copy(original);
    copy.Append(4);
    EXPECT_EQ(copy.GetLength(), 4);
    EXPECT_EQ(copy.GetLast(), 4);
    EXPECT_EQ(original.GetLength(), 3);
    EXPECT_EQ(original.GetLast(), 3);
}

TEST(MutableArraySequence, AppendPrepend) {
    MutableArraySequence<int> seq;
    seq.Append(10);
    seq.Append(20);
    seq.Prepend(5);
    EXPECT_EQ(seq.GetFirst(), 5);
    EXPECT_EQ(seq.GetLast(), 20);
    EXPECT_EQ(seq.GetLength(), 3);
}

TEST(MutableArraySequence, InsertAt) {
    int arr[] = {1, 3};
    MutableArraySequence<int> seq(arr, 2);
    seq.InsertAt(2, 1);
    EXPECT_EQ(seq.Get(1), 2);
    EXPECT_EQ(seq.GetLength(), 3);
}

TEST(MutableArraySequence, GetSubsequence) {
    int arr[] = {0, 1, 2, 3, 4};
    MutableArraySequence<int> seq(arr, 5);
    auto* sub = seq.GetSubsequence(1, 3);
    EXPECT_EQ(sub->GetLength(), 3);
    EXPECT_EQ(sub->Get(0), 1);
    EXPECT_EQ(sub->Get(2), 3);
    delete sub;
}

TEST(MutableArraySequence, RemoveFirst) {
    int arr[] = {1, 2, 3};
    MutableArraySequence<int> seq(arr, 3);
    seq.RemoveFirst();
    EXPECT_EQ(seq.GetFirst(), 2);
    EXPECT_EQ(seq.GetLength(), 2);
}

TEST(MutableArraySequence, RemoveLast) {
    int arr[] = {1, 2, 3};
    MutableArraySequence<int> seq(arr, 3);
    seq.RemoveLast();
    EXPECT_EQ(seq.GetLast(), 2);
    EXPECT_EQ(seq.GetLength(), 2);
}

TEST(MutableArraySequence, RemoveAt) {
    int arr[] = {1, 2, 3};
    MutableArraySequence<int> seq(arr, 3);
    seq.RemoveAt(1);
    EXPECT_EQ(seq.Get(1), 3);
    EXPECT_EQ(seq.GetLength(), 2);
}

TEST(MutableArraySequence, RemoveSingleElement) {
    int arr[] = {42};
    MutableArraySequence<int> seq(arr, 1);
    seq.RemoveFirst();
    EXPECT_EQ(seq.GetLength(), 0);
    EXPECT_THROW(seq.GetFirst(), IndexOutOfRange);
}

TEST(MutableArraySequence, OutOfRange) {
    int arr[] = {1, 2};
    MutableArraySequence<int> seq(arr, 2);
    EXPECT_THROW(seq.Get(-1), IndexOutOfRange);
    EXPECT_THROW(seq.Get(2), IndexOutOfRange);
    EXPECT_THROW(seq.InsertAt(0, -1), IndexOutOfRange);
    EXPECT_THROW(seq.InsertAt(0, 3), IndexOutOfRange);
    EXPECT_THROW(seq.RemoveAt(-1), IndexOutOfRange);
    EXPECT_THROW(seq.RemoveAt(2), IndexOutOfRange);
}

TEST(MutableListSequence, DefaultConstructor) {
    MutableListSequence<int> seq;
    EXPECT_EQ(seq.GetLength(), 0);
    EXPECT_THROW(seq.GetFirst(), IndexOutOfRange);
    EXPECT_THROW(seq.GetLast(), IndexOutOfRange);
}

TEST(MutableListSequence, ConstructFromArray) {
    int arr[] = {10, 20, 30};
    MutableListSequence<int> seq(arr, 3);
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.GetFirst(), 10);
    EXPECT_EQ(seq.GetLast(), 30);
    EXPECT_EQ(seq.Get(1), 20);
}

TEST(MutableListSequence, CopyConstructor) {
    int arr[] = {1, 2, 3};
    MutableListSequence<int> original(arr, 3);
    MutableListSequence<int> copy(original);
    copy.Append(4);
    EXPECT_EQ(copy.GetLength(), 4);
    EXPECT_EQ(copy.GetLast(), 4);
    EXPECT_EQ(original.GetLength(), 3);
    EXPECT_EQ(original.GetLast(), 3);
}

TEST(MutableListSequence, AppendPrepend) {
    MutableListSequence<int> seq;
    seq.Append(10);
    seq.Append(20);
    seq.Prepend(5);
    EXPECT_EQ(seq.GetFirst(), 5);
    EXPECT_EQ(seq.GetLast(), 20);
    EXPECT_EQ(seq.GetLength(), 3);
}

TEST(MutableListSequence, InsertAt) {
    int arr[] = {1, 3};
    MutableListSequence<int> seq(arr, 2);
    seq.InsertAt(2, 1);
    EXPECT_EQ(seq.Get(1), 2);
    EXPECT_EQ(seq.GetLength(), 3);
}

TEST(MutableListSequence, GetSubsequence) {
    int arr[] = {0, 1, 2, 3, 4};
    MutableListSequence<int> seq(arr, 5);
    auto* sub = seq.GetSubsequence(1, 3);
    EXPECT_EQ(sub->GetLength(), 3);
    EXPECT_EQ(sub->Get(0), 1);
    EXPECT_EQ(sub->Get(2), 3);
    delete sub;
}

TEST(MutableListSequence, RemoveFirst) {
    int arr[] = {1, 2, 3};
    MutableListSequence<int> seq(arr, 3);
    seq.RemoveFirst();
    EXPECT_EQ(seq.GetFirst(), 2);
    EXPECT_EQ(seq.GetLength(), 2);
}

TEST(MutableListSequence, RemoveLast) {
    int arr[] = {1, 2, 3};
    MutableListSequence<int> seq(arr, 3);
    seq.RemoveLast();
    EXPECT_EQ(seq.GetLast(), 2);
    EXPECT_EQ(seq.GetLength(), 2);
}

TEST(MutableListSequence, RemoveAt) {
    int arr[] = {1, 2, 3};
    MutableListSequence<int> seq(arr, 3);
    seq.RemoveAt(1);
    EXPECT_EQ(seq.Get(1), 3);
    EXPECT_EQ(seq.GetLength(), 2);
}

TEST(MutableListSequence, RemoveSingleElement) {
    int arr[] = {42};
    MutableListSequence<int> seq(arr, 1);
    seq.RemoveFirst();
    EXPECT_EQ(seq.GetLength(), 0);
    EXPECT_THROW(seq.GetFirst(), IndexOutOfRange);
}

TEST(MutableListSequence, OutOfRange) {
    int arr[] = {1, 2};
    MutableListSequence<int> seq(arr, 2);
    EXPECT_THROW(seq.Get(-1), IndexOutOfRange);
    EXPECT_THROW(seq.Get(2), IndexOutOfRange);
    EXPECT_THROW(seq.InsertAt(0, -1), IndexOutOfRange);
    EXPECT_THROW(seq.InsertAt(0, 3), IndexOutOfRange);
    EXPECT_THROW(seq.RemoveAt(-1), IndexOutOfRange);
    EXPECT_THROW(seq.RemoveAt(2), IndexOutOfRange);
}

TEST(MutableListSequence, OperatorBrackets) {
    int arr[] = {5, 6, 7};
    MutableListSequence<int> seq(arr, 3);
    EXPECT_EQ(seq[0], 5);
    EXPECT_EQ(seq[2], 7);
    seq[1] = 99;
    EXPECT_EQ(seq.Get(1), 99);
}

TEST(MutableListSequence, OperatorBracketsOutOfRange) {
    MutableListSequence<int> seq;
    EXPECT_THROW(seq[0], IndexOutOfRange);
    int arr[] = {1};
    MutableListSequence<int> seq2(arr, 1);
    EXPECT_THROW(seq2[-1], IndexOutOfRange);
    EXPECT_THROW(seq2[1], IndexOutOfRange);
}

//immutable arraysequence

TEST(ImmutableArraySequence, DefaultConstructor) {
    ImmutableArraySequence<int> seq;
    EXPECT_EQ(seq.GetLength(), 0);
    EXPECT_THROW(seq.GetFirst(), IndexOutOfRange);
    EXPECT_THROW(seq.GetLast(), IndexOutOfRange);
}

TEST(ImmutableArraySequence, ConstructFromArray) {
    int arr[] = {10, 20, 30};
    ImmutableArraySequence<int> seq(arr, 3);
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.GetFirst(), 10);
    EXPECT_EQ(seq.GetLast(), 30);
    EXPECT_EQ(seq.Get(1), 20);
}

TEST(ImmutableArraySequence, CopyConstructor) {
    int arr[] = {1, 2, 3};
    ImmutableArraySequence<int> original(arr, 3);
    ImmutableArraySequence<int> copy(original);
    auto* changed = copy.Append(4);
    EXPECT_EQ(changed->GetLength(), 4);
    EXPECT_EQ(changed->GetLast(), 4);
    EXPECT_EQ(original.GetLength(), 3);
    EXPECT_EQ(original.GetLast(), 3);
    delete changed;
}

TEST(ImmutableArraySequence, AppendPrepend) {
    ImmutableArraySequence<int> seq;
    auto* seq1 = seq.Append(10);
    auto* seq2 = seq1->Append(20);
    auto* seq3 = seq2->Prepend(5);
    EXPECT_EQ(seq3->GetFirst(), 5);
    EXPECT_EQ(seq3->GetLast(), 20);
    EXPECT_EQ(seq3->GetLength(), 3);
    delete seq1;
    delete seq2;
    delete seq3;
}

TEST(ImmutableArraySequence, InsertAt) {
    int arr[] = {1, 3};
    ImmutableArraySequence<int> seq(arr, 2);
    auto* changed = seq.InsertAt(2, 1);
    EXPECT_EQ(changed->Get(1), 2);
    EXPECT_EQ(changed->GetLength(), 3);
    delete changed;
}

TEST(ImmutableArraySequence, GetSubsequence) {
    int arr[] = {0, 1, 2, 3, 4};
    ImmutableArraySequence<int> seq(arr, 5);
    auto* sub = seq.GetSubsequence(1, 3);
    EXPECT_EQ(sub->GetLength(), 3);
    EXPECT_EQ(sub->Get(0), 1);
    EXPECT_EQ(sub->Get(2), 3);
    delete sub;
}

TEST(ImmutableArraySequence, RemoveFirst) {
    int arr[] = {1, 2, 3};
    ImmutableArraySequence<int> seq(arr, 3);
    auto* changed = seq.RemoveFirst();
    EXPECT_EQ(changed->GetFirst(), 2);
    EXPECT_EQ(changed->GetLength(), 2);
    delete changed;
}

TEST(ImmutableArraySequence, RemoveLast) {
    int arr[] = {1, 2, 3};
    ImmutableArraySequence<int> seq(arr, 3);
    auto* changed = seq.RemoveLast();
    EXPECT_EQ(changed->GetLast(), 2);
    EXPECT_EQ(changed->GetLength(), 2);
    delete changed;
}

TEST(ImmutableArraySequence, RemoveAt) {
    int arr[] = {1, 2, 3};
    ImmutableArraySequence<int> seq(arr, 3);
    auto* changed = seq.RemoveAt(1);
    EXPECT_EQ(changed->Get(1), 3);
    EXPECT_EQ(changed->GetLength(), 2);
    delete changed;
}

TEST(ImmutableArraySequence, RemoveSingleElement) {
    int arr[] = {42};
    ImmutableArraySequence<int> seq(arr, 1);
    auto* changed = seq.RemoveFirst();
    EXPECT_EQ(changed->GetLength(), 0);
    EXPECT_THROW(changed->GetFirst(), IndexOutOfRange);
    delete changed;
}

TEST(ImmutableArraySequence, OutOfRange) {
    int arr[] = {1, 2};
    ImmutableArraySequence<int> seq(arr, 2);
    EXPECT_THROW(seq.Get(-1), IndexOutOfRange);
    EXPECT_THROW(seq.Get(2), IndexOutOfRange);
    EXPECT_THROW(seq.InsertAt(0, -1), IndexOutOfRange);
    EXPECT_THROW(seq.InsertAt(0, 3), IndexOutOfRange);
    EXPECT_THROW(seq.RemoveAt(-1), IndexOutOfRange);
    EXPECT_THROW(seq.RemoveAt(2), IndexOutOfRange);
}


//immutable listsequence

TEST(ImmutableListSequence, DefaultConstructor) {
    ImmutableListSequence<int> seq;
    EXPECT_EQ(seq.GetLength(), 0);
    EXPECT_THROW(seq.GetFirst(), IndexOutOfRange);
    EXPECT_THROW(seq.GetLast(), IndexOutOfRange);
}

TEST(ImmutableListSequence, ConstructFromArray) {
    int arr[] = {10, 20, 30};
    ImmutableListSequence<int> seq(arr, 3);
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.GetFirst(), 10);
    EXPECT_EQ(seq.GetLast(), 30);
    EXPECT_EQ(seq.Get(1), 20);
}

TEST(ImmutableListSequence, CopyConstructor) {
    int arr[] = {1, 2, 3};
    ImmutableListSequence<int> original(arr, 3);
    ImmutableListSequence<int> copy(original);
    auto* changed = copy.Append(4);
    EXPECT_EQ(changed->GetLength(), 4);
    EXPECT_EQ(changed->GetLast(), 4);
    EXPECT_EQ(original.GetLength(), 3);
    EXPECT_EQ(original.GetLast(), 3);
    delete changed;
}

TEST(ImmutableListSequence, AppendPrepend) {
    ImmutableListSequence<int> seq;
    auto* seq1 = seq.Append(10);
    auto* seq2 = seq1->Append(20);
    auto* seq3 = seq2->Prepend(5);
    EXPECT_EQ(seq3->GetFirst(), 5);
    EXPECT_EQ(seq3->GetLast(), 20);
    EXPECT_EQ(seq3->GetLength(), 3);
    delete seq1;
    delete seq2;
    delete seq3;
}

TEST(ImmutableListSequence, InsertAt) {
    int arr[] = {1, 3};
    ImmutableListSequence<int> seq(arr, 2);
    auto* changed = seq.InsertAt(2, 1);
    EXPECT_EQ(changed->Get(1), 2);
    EXPECT_EQ(changed->GetLength(), 3);
    delete changed;
}

TEST(ImmutableListSequence, GetSubsequence) {
    int arr[] = {0, 1, 2, 3, 4};
    ImmutableListSequence<int> seq(arr, 5);
    auto* sub = seq.GetSubsequence(1, 3);
    EXPECT_EQ(sub->GetLength(), 3);
    EXPECT_EQ(sub->Get(0), 1);
    EXPECT_EQ(sub->Get(2), 3);
    delete sub;
}

TEST(ImmutableListSequence, RemoveFirst) {
    int arr[] = {1, 2, 3};
    ImmutableListSequence<int> seq(arr, 3);
    auto* changed = seq.RemoveFirst();
    EXPECT_EQ(changed->GetFirst(), 2);
    EXPECT_EQ(changed->GetLength(), 2);
    delete changed;
}

TEST(ImmutableListSequence, RemoveLast) {
    int arr[] = {1, 2, 3};
    ImmutableListSequence<int> seq(arr, 3);
    auto* changed = seq.RemoveLast();
    EXPECT_EQ(changed->GetLast(), 2);
    EXPECT_EQ(changed->GetLength(), 2);
    delete changed;
}

TEST(ImmutableListSequence, RemoveAt) {
    int arr[] = {1, 2, 3};
    ImmutableListSequence<int> seq(arr, 3);
    auto* changed = seq.RemoveAt(1);
    EXPECT_EQ(changed->Get(1), 3);
    EXPECT_EQ(changed->GetLength(), 2);
    delete changed;
}

TEST(ImmutableListSequence, RemoveSingleElement) {
    int arr[] = {42};
    ImmutableListSequence<int> seq(arr, 1);
    auto* changed = seq.RemoveFirst();
    EXPECT_EQ(changed->GetLength(), 0);
    EXPECT_THROW(changed->GetFirst(), IndexOutOfRange);
    delete changed;
}

TEST(ImmutableListSequence, OutOfRange) {
    int arr[] = {1, 2};
    ImmutableListSequence<int> seq(arr, 2);
    EXPECT_THROW(seq.Get(-1), IndexOutOfRange);
    EXPECT_THROW(seq.Get(2), IndexOutOfRange);
    EXPECT_THROW(seq.InsertAt(0, -1), IndexOutOfRange);
    EXPECT_THROW(seq.InsertAt(0, 3), IndexOutOfRange);
    EXPECT_THROW(seq.RemoveAt(-1), IndexOutOfRange);
    EXPECT_THROW(seq.RemoveAt(2), IndexOutOfRange);
}

TEST(ImmutableListSequence, OperatorBrackets) {
    int arr[] = {5, 6, 7};
    ImmutableListSequence<int> seq(arr, 3);
    EXPECT_EQ(seq[0], 5);
    EXPECT_EQ(seq[2], 7);
}

TEST(ImmutableListSequence, OperatorBracketsOutOfRange) {
    ImmutableListSequence<int> seq;
    EXPECT_THROW(seq[0], IndexOutOfRange);
    int arr[] = {1};
    ImmutableListSequence<int> seq2(arr, 1);
    EXPECT_THROW(seq2[-1], IndexOutOfRange);
    EXPECT_THROW(seq2[1], IndexOutOfRange);
}